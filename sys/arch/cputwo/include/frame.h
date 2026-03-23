#ifndef _CPUTWO_FRAME_H_
#define _CPUTWO_FRAME_H_

#ifndef _LOCORE

#include <sys/signal.h>
#include <sys/ucontext.h>

/*
 * Trap frame. Pushed onto the kernel stack on a trap.
 */
typedef struct trapframe {
	register_t tf_status;   /* saved STATUS register */
	register_t tf_r0;
	register_t tf_r1;
	register_t tf_r2;
	register_t tf_r3;
	register_t tf_r4;
	register_t tf_r5;
	register_t tf_r6;
	register_t tf_r7;
	register_t tf_r8;
	register_t tf_r9;
	register_t tf_r10;
	register_t tf_r11;
	register_t tf_r12;
	register_t tf_sp;
	register_t tf_lr;
	register_t tf_pc;
} trapframe_t;

#define tf_r13 tf_sp
#define tf_r14 tf_lr
#define tf_r15 tf_pc

#define TRAP_USERMODE(tf)   (((tf)->tf_status & 0x01) == 0)

#if defined(COMPAT_16) || defined(__minix)
struct sigframe_sigcontext {
#if defined(__minix)
	struct sigcontext *sf_scp;
#endif
	struct sigcontext sf_sc;
};
#endif

struct sigframe_siginfo {
	siginfo_t       sf_si;
	ucontext_t      sf_uc;
};

#ifdef _KERNEL
#define lwp_trapframe(l)        ((l)->l_md.md_tf)
#define lwp_settrapframe(l, tf) ((l)->l_md.md_tf = (tf))
#endif

#endif /* _LOCORE */

#endif /* _CPUTWO_FRAME_H_ */
