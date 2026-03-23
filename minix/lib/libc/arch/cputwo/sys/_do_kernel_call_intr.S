#include <minix/ipcconst.h>
#include <machine/asm.h>

ENTRY(_do_kernel_call_intr)
	// r0 already holds msg ptr, move to r1
	mov	r1, r0
	movi	r0, KERVEC_INTR		// r0 = syscall vector
	syscall				// trap to kernel
	mov	r15, r14		// return
