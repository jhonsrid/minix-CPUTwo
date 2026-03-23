#include <machine/asm.h>
#include <minix/ipcconst.h>
#include "archconst.h"

//
// CPUTwo exception/interrupt/syscall handling for Minix kernel
//
// On exception entry, hardware:
//   - Saves STATUS -> ESTATUS
//   - Saves PC (or PC+4 for SYSCALL) -> EPC
//   - Saves flags -> EFLAGS
//   - Writes cause code -> CAUSE
//   - Sets STATUS = 0x01 (supervisor, IE=0)
//   - Jumps to EVEC[cause * 4]
//
// All GPRs are untouched by hardware; software must save them.
//

.text

// Macro to load a 32-bit address into a register
.macro load32 reg, hi, lo
    movi \reg, \lo
    movhi \reg, \hi
.endm

//
// Exception vector table. Each entry is a branch to the handler.
// Loaded into EVEC register during protect.c init.
//
.globl cputwo_exc_table
.balign 4
cputwo_exc_table:
	ba	exc_illegal		// 0x00: illegal instruction
	ba	exc_misalign		// 0x01: misaligned access
	ba	exc_buserr		// 0x02: bus error
	ba	exc_syscall		// 0x03: syscall
	ba	exc_divzero		// 0x04: divide by zero
	ba	exc_halt		// 0x05: halt
	ba	exc_irq			// 0x06: hardware interrupt
	ba	exc_ipgfault		// 0x07: instruction page fault
	ba	exc_lpgfault		// 0x08: load page fault
	ba	exc_spgfault		// 0x09: store page fault

//
// save_context: save all user registers to proc table
//
// Called via "jmp r14, save_context". The caller must first save user lr
// to the kernel stack at [sp - 12] before the jmp clobbers r14.
//
// On entry:
//   - r14 = return address (set by jmp)
//   - user lr has been spilled to [sp - 12] by caller
//   - all other user GPRs are intact
//
// We use r0 and r12 as scratch after spilling them.
//
// stackframe_s layout offsets:
//   retreg(r0)=0, r1=4, r2=8, r3=12, r4=16, r5=20, r6=24, r7=28,
//   r8=32, r9=36, r10=40, r11=44, r12=48, sp=52, lr=56,
//   pc=60, status=64, eflags=68
//
save_context:
	// Spill user r0 and r12 to kernel stack so we have scratch regs
	sw	r0, sp, -4		// save user r0
	sw	r12, sp, -8		// save user r12

	// Load __cputwo_proc_ptr into r12
	movi	r12, %lo(__cputwo_proc_ptr)
	movhi	r12, %hi(__cputwo_proc_ptr)
	lw	r12, r12, 0		// r12 = pointer to current proc's stackframe

	// Save user r1-r11 directly
	sw	r1, r12, 4
	sw	r2, r12, 8
	sw	r3, r12, 12
	sw	r4, r12, 16
	sw	r5, r12, 20
	sw	r6, r12, 24
	sw	r7, r12, 28
	sw	r8, r12, 32
	sw	r9, r12, 36
	sw	r10, r12, 40
	sw	r11, r12, 44

	// Restore and save user r0 from stack spill
	lw	r0, sp, -4
	sw	r0, r12, 0		// retreg = user r0

	// Restore and save user r12 from stack spill
	lw	r0, sp, -8
	sw	r0, r12, 48		// r12

	// Save user sp (r13) -- untouched by hardware
	sw	r13, r12, 52		// sp

	// NOTE: user lr is NOT saved here; it was clobbered by the jmp.
	// The caller spilled user lr to [sp - 12] and will patch it
	// into the stackframe (offset 56) after we return.

	// Save EPC as user's PC
	load32	r0, 0x03FF, 0xF000	// REG_EPC
	lw	r0, r0, 0
	sw	r0, r12, 60		// pc

	// Save ESTATUS as user's status
	load32	r0, 0x03FF, 0xF014	// REG_ESTATUS
	lw	r0, r0, 0
	sw	r0, r12, 64		// status

	// Save EFLAGS
	load32	r0, 0x03FF, 0xF004	// REG_EFLAGS
	lw	r0, r0, 0
	sw	r0, r12, 68		// eflags

	// Return to caller via lr (set by the jmp instruction)
	mov	r15, r14

//
// Exception handlers - all call into C exception_handler()
//
// Each handler:
//   1. Saves user lr to kernel stack at [sp - 12]
//   2. Calls save_context via jmp (clobbers lr with return addr)
//   3. Patches user lr into stackframe from the kernel stack spill
//   4. Calls the C handler
//   5. Jumps to switch_to_user
//

exc_illegal:
exc_misalign:
exc_buserr:
exc_divzero:
exc_halt:
exc_ipgfault:
exc_lpgfault:
exc_spgfault:
	// Save user lr to kernel stack before jmp clobbers it
	sw	r14, sp, -12
	jmp	r14, save_context
	// Patch user lr into proc stackframe
	lw	r0, sp, -12
	movi	r1, %lo(__cputwo_proc_ptr)
	movhi	r1, %hi(__cputwo_proc_ptr)
	lw	r1, r1, 0
	sw	r0, r1, 56		// fix up lr in stackframe
	// Read cause code
	load32	r0, 0x03FF, 0xF00C	// REG_CAUSE
	lw	r0, r0, 0
	// Call C exception handler
	jmp	lr, _C_LABEL(exception_handler)
	// Return to user via switch_to_user
	ba	_C_LABEL(switch_to_user)

//
// Hardware interrupt handler
//
exc_irq:
	sw	r14, sp, -12
	jmp	r14, save_context
	lw	r0, sp, -12
	movi	r1, %lo(__cputwo_proc_ptr)
	movhi	r1, %hi(__cputwo_proc_ptr)
	lw	r1, r1, 0
	sw	r0, r1, 56		// fix up lr
	// Call C interrupt handler
	jmp	lr, _C_LABEL(irq_handler)
	ba	_C_LABEL(switch_to_user)

//
// Syscall handler - dispatches IPC and kernel calls
// On SYSCALL entry: r0 = syscall vector number
//   r1 = first arg, r2 = second arg, r3 = third arg
//
exc_syscall:
	sw	r14, sp, -12
	jmp	r14, save_context
	lw	r0, sp, -12
	movi	r1, %lo(__cputwo_proc_ptr)
	movhi	r1, %hi(__cputwo_proc_ptr)
	lw	r1, r1, 0
	sw	r0, r1, 56		// fix up lr

	// Reload cause to verify it's a syscall
	load32	r4, 0x03FF, 0xF00C	// REG_CAUSE
	lw	r4, r4, 0

	// Reload saved r0 from proc (the syscall vector number)
	movi	r5, %lo(__cputwo_proc_ptr)
	movhi	r5, %hi(__cputwo_proc_ptr)
	lw	r5, r5, 0
	lw	r0, r5, 0		// r0 = saved user r0 (syscall vector)
	lw	r1, r5, 4		// r1 = saved user r1
	lw	r2, r5, 8		// r2 = saved user r2
	lw	r3, r5, 12		// r3 = saved user r3

	cmpi	r0, KERVEC_INTR
	beq	kernel_call_entry
	cmpi	r0, IPCVEC_INTR
	beq	ipc_entry

	// Unknown syscall vector - return -1
	movi	r0, -1
	movi	r5, %lo(__cputwo_proc_ptr)
	movhi	r5, %hi(__cputwo_proc_ptr)
	lw	r5, r5, 0
	sw	r0, r5, 0		// store return value in retreg
	ba	_C_LABEL(switch_to_user)

kernel_call_entry:
	// r1 = message pointer
	mov	r0, r1
	jmp	lr, _C_LABEL(kernel_call)
	ba	_C_LABEL(switch_to_user)

ipc_entry:
	// r1 = src_dst, r2 = m_ptr, r3 = bit_map
	mov	r0, r1			// src_dst_e
	mov	r1, r2			// m_ptr
	mov	r2, r3			// bit_map
	jmp	lr, _C_LABEL(do_ipc)
	// Store IPC return value
	movi	r5, %lo(__cputwo_proc_ptr)
	movhi	r5, %hi(__cputwo_proc_ptr)
	lw	r5, r5, 0
	sw	r0, r5, 0		// store in retreg (r0)
	ba	_C_LABEL(switch_to_user)

//
// restore_user_context
// Called from proc.c switch_to_user() after arch_finish_switch_to_user().
// r0 = pointer to proc whose context to restore.
//
.globl restore_user_context
restore_user_context:
	// r0 = pointer to proc to restore
	// Restore user registers from proc's stackframe

	// First set EPC from saved pc
	lw	r1, r0, 60		// pc
	load32	r2, 0x03FF, 0xF000	// REG_EPC
	sw	r1, r2, 0

	// Set ESTATUS from saved status
	lw	r1, r0, 64		// status
	load32	r2, 0x03FF, 0xF014	// REG_ESTATUS
	sw	r1, r2, 0

	// Set EFLAGS from saved eflags
	lw	r1, r0, 68		// eflags
	load32	r2, 0x03FF, 0xF004	// REG_EFLAGS
	sw	r1, r2, 0

	// Restore GPRs
	lw	r1, r0, 4
	lw	r2, r0, 8
	lw	r3, r0, 12
	lw	r4, r0, 16
	lw	r5, r0, 20
	lw	r6, r0, 24
	lw	r7, r0, 28
	lw	r8, r0, 32
	lw	r9, r0, 36
	lw	r10, r0, 40
	lw	r11, r0, 44
	lw	r12, r0, 48
	lw	r13, r0, 52		// sp
	lw	r14, r0, 56		// lr

	// Restore r0 last
	lw	r0, r0, 0		// retreg

	// Return to user mode
	sysret
