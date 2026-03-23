#include <machine/asm.h>
#include <ucontextoffsets.h>

// int getcontext(ucontext_t *ucp)
ENTRY(getcontext)
ENTRY(_getcontext)
	mov	r3, r14			// Save return address

	// Check null pointer
	cmpi	r0, 0
	bne	gc_notnull
	movi	r0, -1			// return -1
	mov	r15, r14
gc_notnull:
	// Save callee-saved registers
	mov	r14, r3			// Restore lr
	sw	r14, r0, LRREG		// Save lr
	sw	r14, r0, PCREG		// Save return address as PC
	sw	sp, r0, SPREG		// Save sp
	sw	r11, r0, FPREG		// Save fp (r11)
	sw	r4, r0, REG4
	sw	r5, r0, REG5
	sw	r6, r0, REG6
	sw	r7, r0, REG7
	sw	r8, r0, REG8
	sw	r9, r0, REG9
	sw	r10, r0, REG10

	// Set magic value
	movi	r1, %lo(MCF_MAGIC)
	movhi	r1, %hi(MCF_MAGIC)
	sw	r1, r0, MAGIC

	// Return 0
	movi	r1, 0
	sw	r1, r0, REG0		// stored return value = 0
	movi	r0, 0			// actual return value = 0
	mov	r15, r14

// int setcontext(const ucontext_t *ucp)
ENTRY(setcontext)
	// Check null pointer
	cmpi	r0, 0
	bne	sc_notnull
	movi	r0, -1
	mov	r15, r14
sc_notnull:
	// Check magic
	lw	r1, r0, MAGIC
	movi	r2, %lo(MCF_MAGIC)
	movhi	r2, %hi(MCF_MAGIC)
	cmp	r1, r2
	beq	sc_valid
	movi	r0, -1
	mov	r15, r14
sc_valid:
	// Restore registers
	lw	r4, r0, REG4
	lw	r5, r0, REG5
	lw	r6, r0, REG6
	lw	r7, r0, REG7
	lw	r8, r0, REG8
	lw	r9, r0, REG9
	lw	r10, r0, REG10
	lw	r11, r0, FPREG		// fp
	lw	sp, r0, SPREG
	lw	r14, r0, LRREG
	mov	r3, r0
	lw	r0, r3, REG0		// Restore r0
	lw	r3, r3, PCREG		// Load saved PC
	mov	r15, r3			// Jump to saved PC

// void ctx_start()
ENTRY(ctx_start)
	mov	r0, r4
	movi	r1, %lo(resumecontext)
	movhi	r1, %hi(resumecontext)
	mov	r15, r1
