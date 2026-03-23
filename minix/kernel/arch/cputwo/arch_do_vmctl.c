/* CPUTwo VM control kernel call */
#include "kernel/kernel.h"
#include "kernel/proc.h"
#include "kernel/vm.h"
#include <machine/vm.h>
#include "archconst.h"
#include "arch_proto.h"

static void set_satp(struct proc *p, u32_t satp)
{
	p->p_seg.p_satp = satp;
	p->p_seg.p_satp_v = (u32_t *)((satp & CPUTWO_SATP_PPN_MASK) << 12);
}

int arch_do_vmctl(message *m_ptr, struct proc *p)
{
	switch (m_ptr->SVMCTL_PARAM) {
	case VMCTL_GET_PDBR:
		m_ptr->SVMCTL_VALUE = p->p_seg.p_satp;
		return OK;

	case VMCTL_SETADDRSPACE:
		set_satp(p, m_ptr->SVMCTL_VALUE);
		if (p == get_cpulocal_var(proc_ptr))
			switch_address_space(p);
		return OK;

	case VMCTL_FLUSHTLB:
		/* Flush TLB via SFENCE */
		__asm volatile(".word 0x3E000000");
		return OK;
	}

	return EINVAL;
}
