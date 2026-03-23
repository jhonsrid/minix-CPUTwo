/*
 * CPUTwo trap/exception definitions
 */

#ifndef _CPUTWO_TRAP_H_
#define _CPUTWO_TRAP_H_

/* CPUTwo exception cause codes (from EVEC table) */
#define T_ILLEGAL       0x00    /* Illegal instruction */
#define T_MISALIGN      0x01    /* Misaligned memory access */
#define T_BUSERR        0x02    /* Bus error (out of range) */
#define T_SYSCALL       0x03    /* SYSCALL instruction */
#define T_DIVZERO       0x04    /* Divide by zero */
#define T_HALT          0x05    /* HALT instruction */
#define T_IRQ           0x06    /* Hardware interrupt */
#define T_IPGFAULT      0x07    /* Instruction page fault */
#define T_LPGFAULT      0x08    /* Load page fault */
#define T_SPGFAULT      0x09    /* Store page fault */

/* Breakpoint (undefined instruction encoding for debugger use) */
#define KERNEL_BREAKPOINT   0xFFFFFFFF
#define GDB_BREAKPOINT      0xFFFFFFFF
#define USER_BREAKPOINT     GDB_BREAKPOINT

#define KBPT_ASM        ".word 0xffffffff"

#endif /* _CPUTWO_TRAP_H_ */
