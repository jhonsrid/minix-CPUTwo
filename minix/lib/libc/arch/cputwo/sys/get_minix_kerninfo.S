#include <minix/ipcconst.h>
#include <machine/asm.h>

ENTRY(get_minix_kerninfo)
	sw	r0, sp, -4		// save return ptr
	subi	sp, sp, 4
	movi	r2, 0			// clear args
	movi	r3, 0
	movi	r1, MINIX_KERNINFO	// r1 = call_nr
	movi	r0, IPCVEC_INTR		// r0 = IPC vector
	syscall
	// r1 = kerninfo pointer returned by kernel
	lw	r2, sp, 0		// restore return ptr
	addi	sp, sp, 4
	sw	r1, r2, 0		// *ptr = kerninfo
	mov	r15, r14
