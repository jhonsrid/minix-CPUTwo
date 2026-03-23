#ifndef _CPUTWO_STACKFRAME_H
#define _CPUTWO_STACKFRAME_H

#include <sys/types.h>

typedef u32_t reg_t;         /* machine register */

struct stackframe_s {
	reg_t retreg;                 /*  r0 */
	reg_t r1;
	reg_t r2;
	reg_t r3;
	reg_t r4;
	reg_t r5;
	reg_t r6;
	reg_t r7;
	reg_t r8;
	reg_t r9;
	reg_t r10;
	reg_t r11;
	reg_t r12;
	reg_t sp;                     /*  r13 */
	reg_t lr;                     /*  r14 */
	reg_t pc;                     /*  r15 */
	reg_t status;                 /*  STATUS register */
	reg_t eflags;                 /*  saved flags */
};

#endif /* _CPUTWO_STACKFRAME_H */
