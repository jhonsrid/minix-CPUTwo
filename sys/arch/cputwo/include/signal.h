#ifndef _CPUTWO_SIGNAL_H_
#define _CPUTWO_SIGNAL_H_

#include <sys/featuretest.h>
#include <sys/sigtypes.h>

#ifndef _LOCORE
typedef int sig_atomic_t;
#endif

#if defined(_NETBSD_SOURCE)

#ifndef _LOCORE
#if defined(__LIBC12_SOURCE__) || defined(_KERNEL)
struct sigcontext13 {
	int	sc_onstack;
	int	sc_mask;
	unsigned int sc_status;
	unsigned int sc_r0;
	unsigned int sc_r1;
	unsigned int sc_r2;
	unsigned int sc_r3;
	unsigned int sc_r4;
	unsigned int sc_r5;
	unsigned int sc_r6;
	unsigned int sc_r7;
	unsigned int sc_r8;
	unsigned int sc_r9;
	unsigned int sc_r10;
	unsigned int sc_r11;
	unsigned int sc_r12;
	unsigned int sc_sp;
	unsigned int sc_lr;
	unsigned int sc_pc;
};
#endif

struct sigcontext {
	int	sc_onstack;
	int	__sc_mask13;
	unsigned int sc_status;
	unsigned int sc_r0;
	unsigned int sc_r1;
	unsigned int sc_r2;
	unsigned int sc_r3;
	unsigned int sc_r4;
	unsigned int sc_r5;
	unsigned int sc_r6;
	unsigned int sc_r7;
	unsigned int sc_r8;
	unsigned int sc_r9;
	unsigned int sc_r10;
	unsigned int sc_r11;
	unsigned int sc_r12;
	unsigned int sc_sp;
	unsigned int sc_lr;
	unsigned int sc_pc;
	sigset_t sc_mask;
#ifdef __minix
#define SC_MAGIC       0xc0ffee2
	int sc_magic;
	int sc_flags;
	int trap_style;
#endif
};
#endif /* !_LOCORE */

#endif /* _NETBSD_SOURCE */

#if defined(__minix)
__BEGIN_DECLS
int sigreturn(struct sigcontext *_scp);
__END_DECLS
#endif

#endif /* _CPUTWO_SIGNAL_H_ */
