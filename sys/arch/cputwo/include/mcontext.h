#ifndef _CPUTWO_MCONTEXT_H_
#define _CPUTWO_MCONTEXT_H_

#include <sys/stdint.h>

/*
 * General register state: r0-r14 + pc + status = 17 registers
 */
#define _NGREG      17
typedef unsigned int    __greg_t;
typedef __greg_t        __gregset_t[_NGREG];

#define _REG_R0     0
#define _REG_R1     1
#define _REG_R2     2
#define _REG_R3     3
#define _REG_R4     4
#define _REG_R5     5
#define _REG_R6     6
#define _REG_R7     7
#define _REG_R8     8
#define _REG_R9     9
#define _REG_R10    10
#define _REG_R11    11
#define _REG_R12    12
#define _REG_R13    13
#define _REG_R14    14
#define _REG_R15    15
#define _REG_STATUS 16

/* Convenience synonyms */
#define _REG_SP     _REG_R13
#define _REG_LR     _REG_R14
#define _REG_PC     _REG_R15
#define _REG_FP     _REG_R11

/* No FPU on CPUTwo */
typedef struct {
	unsigned int __fp_unused;
} __fpregset_t;

typedef struct {
	__gregset_t     __gregs;
	__fpregset_t    __fpu;
	__greg_t        _mc_tlsbase;
#if defined(__minix)
	int mc_flags;
	int mc_magic;
#endif
} mcontext_t, mcontext32_t;

#define _UC_SETSTACK    0x00020000
#define _UC_CLRSTACK    0x00040000
#define _UC_TLSBASE     0x00080000

#define _UC_MACHINE_PAD 2

#define _UC_MACHINE_SP(uc)      ((uc)->uc_mcontext.__gregs[_REG_SP])
#define _UC_MACHINE_PC(uc)      ((uc)->uc_mcontext.__gregs[_REG_PC])
#define _UC_MACHINE_INTRV(uc)   ((uc)->uc_mcontext.__gregs[_REG_R0])

#define _UC_MACHINE_SET_PC(uc, pc)  _UC_MACHINE_PC(uc) = (pc)

#if defined(__minix)
#define _UC_MACHINE_STACK(uc)       ((uc)->uc_mcontext.__gregs[_REG_SP])
#define _UC_MACHINE_SET_STACK(uc, sp)   _UC_MACHINE_STACK(uc) = (sp)

#define _UC_MACHINE_FP(uc)         ((uc)->uc_mcontext.__gregs[_REG_FP])
#define _UC_MACHINE_SET_FP(uc, fp) _UC_MACHINE_FP(uc) = (fp)

#define _UC_MACHINE_LR(uc)         ((uc)->uc_mcontext.__gregs[_REG_LR])
#define _UC_MACHINE_SET_LR(uc, lr) _UC_MACHINE_LR(uc) = (lr)

#define _UC_MACHINE_R0(uc)             ((uc)->uc_mcontext.__gregs[_REG_R0])
#define _UC_MACHINE_SET_R0(uc, setreg) _UC_MACHINE_R0(uc) = (setreg)

#define _UC_MACHINE_R1(uc)             ((uc)->uc_mcontext.__gregs[_REG_R1])
#define _UC_MACHINE_SET_R1(uc, setreg) _UC_MACHINE_R1(uc) = (setreg)

#define _UC_MACHINE_R2(uc)             ((uc)->uc_mcontext.__gregs[_REG_R2])
#define _UC_MACHINE_SET_R2(uc, setreg) _UC_MACHINE_R2(uc) = (setreg)

#define _UC_MACHINE_R3(uc)             ((uc)->uc_mcontext.__gregs[_REG_R3])
#define _UC_MACHINE_SET_R3(uc, setreg) _UC_MACHINE_R3(uc) = (setreg)

#define _UC_MACHINE_R4(uc)             ((uc)->uc_mcontext.__gregs[_REG_R4])
#define _UC_MACHINE_SET_R4(uc, setreg) _UC_MACHINE_R4(uc) = (setreg)
#endif /* defined(__minix) */

#define __UCONTEXT_SIZE 256

#if defined(__minix)
int setmcontext(const mcontext_t *mcp);
int getmcontext(mcontext_t *mcp);
#define MCF_MAGIC 0xc0ffee
#endif

#endif /* _CPUTWO_MCONTEXT_H_ */
