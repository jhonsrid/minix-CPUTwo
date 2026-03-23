#include <machine/asm.h>

.text
.globl MINIX
.globl __k_unpaged_MINIX

// Macro to load a 32-bit address into a register
.macro load32 reg, hi, lo
    movi \reg, \lo
    movhi \reg, \hi
.endm

// Kernel entry point. Called by bootloader/QEMU with kernel loaded at 0x0
MINIX:
__k_unpaged_MINIX:
	// Set up initial kernel stack below MMIO region (0x03EFFFFC)
	load32	sp, 0x03EF, 0xFFFC

	// Call pre_init in C to set up page tables and boot info
	movi	r0, 0
	jmp	lr, _C_LABEL(pre_init)

	// pre_init returns with boot info pointer in r0
	// Set up high kernel stack
	movi	sp, %lo(k_initial_stktop)
	movhi	sp, %hi(k_initial_stktop)

	// Call kmain
	jmp	lr, __k_unpaged_kmain

	// Should not return
	halt

.data
.globl k_initial_stack
.globl k_initial_stktop
.globl k_boot_stack
.globl k_stacks_start
.globl k_stacks

	.balign 4096
k_initial_stack:
	.space 4096
k_initial_stktop:

	.balign 4096
k_boot_stack:
	.space 4096

	.balign 4096
k_stacks_start:
	// Space for CONFIG_MAX_CPUS kernel stacks, 2 pages each
	.space (2 * 4096 * 1)
k_stacks:
	// k_stacks points past the end; get_k_stack_top indexes backward
