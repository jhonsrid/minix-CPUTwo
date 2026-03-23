#include <machine/asm.h>

.text

// Macro to load a 32-bit address into a register
.macro load32 reg, hi, lo
    movi \reg, \lo
    movhi \reg, \hi
.endm

//
// void phys_copy(phys_bytes src, phys_bytes dst, phys_bytes len)
// Copy len bytes from src to dst. All physical addresses.
// r0 = src, r1 = dst, r2 = len
//
.globl phys_copy
.globl __k_unpaged_phys_copy
phys_copy:
__k_unpaged_phys_copy:
	cmpi	r2, 0
	beq	2f
1:
	lbu	r3, r0, 0
	sb	r3, r1, 0
	addi	r0, r0, 1
	addi	r1, r1, 1
	subi	r2, r2, 1
	cmpi	r2, 0
	bne	1b
2:
	movi	r0, 0
	mov	r15, r14		// return

//
// void phys_memset(phys_bytes addr, u32_t pattern, phys_bytes bytes)
// r0 = addr, r1 = pattern (byte), r2 = count
//
.globl phys_memset
.globl __k_unpaged_phys_memset
phys_memset:
__k_unpaged_phys_memset:
	cmpi	r2, 0
	beq	2f
1:
	sb	r1, r0, 0
	addi	r0, r0, 1
	subi	r2, r2, 1
	cmpi	r2, 0
	bne	1b
2:
	movi	r0, 0
	mov	r15, r14		// return

//
// void copy_msg_from_user(message *src, message *dst)
// Copy a Minix message (64 bytes) from user space.
// r0 = user src, r1 = kernel dst
//
.globl copy_msg_from_user
.globl __copy_msg_from_user_end
copy_msg_from_user:
	// Copy 64 bytes = 16 words
	movi	r2, 16
1:
	lw	r3, r0, 0
	sw	r3, r1, 0
	addi	r0, r0, 4
	addi	r1, r1, 4
	subi	r2, r2, 1
	cmpi	r2, 0
	bne	1b
	movi	r0, 0			// success
	mov	r15, r14
__copy_msg_from_user_end:

//
// void copy_msg_to_user(message *src, message *dst)
// Copy a Minix message (64 bytes) to user space.
// r0 = kernel src, r1 = user dst
//
.globl copy_msg_to_user
.globl __copy_msg_to_user_end
copy_msg_to_user:
	movi	r2, 16
1:
	lw	r3, r0, 0
	sw	r3, r1, 0
	addi	r0, r0, 4
	addi	r1, r1, 4
	subi	r2, r2, 1
	cmpi	r2, 0
	bne	1b
	movi	r0, 0
	mov	r15, r14
__copy_msg_to_user_end:

//
// Error handler for bad user message pointer
//
.globl __user_copy_msg_pointer_failure
__user_copy_msg_pointer_failure:
	movi	r0, -1
	mov	r15, r14

//
// void intr_enable(void) -- enable interrupts
//
.globl intr_enable
.globl interrupts_enable
intr_enable:
interrupts_enable:
	load32	r0, 0x03FF, 0xF010	// REG_STATUS
	lw	r1, r0, 0
	ori	r1, r1, 0x02		// set IE bit
	sw	r1, r0, 0
	mov	r15, r14

//
// void intr_disable(void) -- disable interrupts
//
.globl intr_disable
.globl interrupts_disable
intr_disable:
interrupts_disable:
	load32	r0, 0x03FF, 0xF010	// REG_STATUS
	lw	r1, r0, 0
	andi	r1, r1, 0xFFFD		// clear IE bit
	sw	r1, r0, 0
	mov	r15, r14
