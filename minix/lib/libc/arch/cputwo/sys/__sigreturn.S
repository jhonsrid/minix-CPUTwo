#include <machine/asm.h>

ENTRY(__sigreturn)
	lw	r0, sp, 0	// load sigframe.sf_scp into r0
	addi	sp, sp, 4	// pop
	movi	r1, %lo(sigreturn)
	movhi	r1, %hi(sigreturn)
	mov	r15, r1		// jump to sigreturn
