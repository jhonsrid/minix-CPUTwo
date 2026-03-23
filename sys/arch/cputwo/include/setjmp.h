#ifndef _CPUTWO_SETJMP_H_
#define _CPUTWO_SETJMP_H_
/*
 * machine/setjmp.h: machine dependent setjmp-related information.
 *
 * CPUTwo jmp_buf layout:
 *   word  0    magic number
 *   word  1    r4
 *   word  2    r5
 *   word  3    r6
 *   word  4    r7
 *   word  5    r8
 *   word  6    r9
 *   word  7    r10
 *   word  8    r11
 *   word  9    r13 (sp)
 *   word 10    r14 (lr)
 *   word 11    signal mask (if setjmp)
 *   word 12    (con't)
 *   word 13    (con't)
 *   word 14    (con't)
 */

#define _JBLEN  32      /* size, in longs, of a jmp_buf */

#define _JB_MAGIC__SETJMP   0x43505500
#define _JB_MAGIC_SETJMP    0x43505501

#define _JB_MAGIC       0
#define _JB_REG_R4      1
#define _JB_REG_R5      2
#define _JB_REG_R6      3
#define _JB_REG_R7      4
#define _JB_REG_R8      5
#define _JB_REG_R9      6
#define _JB_REG_R10     7
#define _JB_REG_R11     8
#define _JB_REG_R13     9
#define _JB_REG_R14     10

#define _JB_SIGMASK     11

#endif /* _CPUTWO_SETJMP_H_ */
