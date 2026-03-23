#ifndef _CPUTWO_TYPES_H
#define _CPUTWO_TYPES_H

#include <minix/sys_config.h>
#include <machine/stackframe.h>
#include <sys/cdefs.h>

typedef struct segframe {
	reg_t	p_satp;		/* page table root (SATP register value) */
	u32_t	*p_satp_v;	/* virtual address of page table root */
	char	*fpu_state;	/* unused on CPUTwo (no FPU) */
} segframe_t;

struct cpu_info {
	u32_t	freq;		/* in MHz */
};

typedef u32_t atomic_t;

#endif /* _CPUTWO_TYPES_H */
