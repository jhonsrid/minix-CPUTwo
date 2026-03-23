/* CPUTwo system-dependent kernel functions */
#include "kernel/kernel.h"
#include "kernel/proc.h"
#include "kernel/system.h"
#include "archconst.h"
#include "arch_proto.h"
#include <machine/vm.h>
#include <string.h>
#include <assert.h>

/*
 * Global pointer to current process's stackframe, kept in sync with
 * the cpulocal proc_ptr. Used by assembly code in mpx.S which cannot
 * easily access cpulocal variables.
 */
struct proc *__cputwo_proc_ptr;

void cpu_identify(void)
{
	/* CPUTwo has no CPUID-like instruction; set defaults */
	machine.board_id = 0;
}

void arch_proc_reset(struct proc *pr)
{
	memset(&pr->p_reg, 0, sizeof(pr->p_reg));
}

void arch_proc_setcontext(struct proc *p, struct stackframe_s *state,
    int isuser, int trapstyle)
{
	if (state != &p->p_reg)
		memcpy(&p->p_reg, state, sizeof(p->p_reg));

	if (isuser) {
		/* Ensure user mode, interrupts enabled */
		p->p_reg.status = CPUTWO_STATUS_IE;
	}

	p->p_misc_flags |= MF_CONTEXT_SET;
}

void arch_set_secondary_ipc_return(struct proc *p, u32_t val)
{
	p->p_reg.r1 = val;
}

void arch_do_syscall(struct proc *p)
{
	/* Execute a deferred kernel call on behalf of process p.
	 * The syscall arguments are in the process's saved registers.
	 * r0=vector, r1=src_dst, r2=m_ptr, r3=bit_map */
	p->p_reg.retreg = do_ipc(p->p_reg.r1, p->p_reg.r2, p->p_reg.r3);
}

struct proc *arch_finish_switch_to_user(void)
{
	struct proc *p;

	p = get_cpulocal_var(proc_ptr);

	/* Update global alias for assembly code */
	__cputwo_proc_ptr = p;

	/* Switch address space to the process's page table */
	switch_address_space(p);

	/* Return pointer to proc whose context should be restored */
	return p;
}

void fpu_init(void)
{
	/* No FPU on CPUTwo */
}

void save_fpu(struct proc *pr)
{
	/* No FPU */
}

int restore_fpu(struct proc *pr)
{
	/* No FPU */
	return 0;
}

void arch_pause(void)
{
	/* CPUTwo has no WFI instruction; busy-wait with interrupts enabled.
	 * The HALT instruction stops execution entirely, so we just spin. */
	volatile u32_t *status = (volatile u32_t *)CPUTWO_REG_STATUS;
	*status |= CPUTWO_STATUS_IE;
}

void arch_ser_init(void)
{
	/* UART already initialized by hardware/QEMU */
}

short cpu_load(void)
{
	return 0;
}

void arch_init(void)
{
	/* Architecture-specific initialization called from main.c */
}

reg_t arch_get_sp(struct proc *p)
{
	return p->p_reg.sp;
}

void do_ser_debug(void)
{
	/* Serial debug: nothing to do */
}

void get_randomness(struct k_randomness *rand, int source)
{
	/* No hardware entropy source on CPUTwo */
}

void save_local_fpu(struct proc *p, int retain)
{
	/* No FPU */
}

void fpu_sigcontext(struct proc *p, struct sigframe_sigcontext *fr, struct sigcontext *sc)
{
	/* No FPU */
}

void cycles_accounting_init(void)
{
	/* No cycle counter on CPUTwo */
}

int arch_phys_map(int index, phys_bytes *addr, phys_bytes *len, int *flags)
{
	/* No additional kernel MMIO mappings needed;
	 * CPUTwo MMIO region bypasses MMU */
	return EINVAL;
}

int arch_phys_map_reply(int index, vir_bytes addr)
{
	return OK;
}

int arch_enable_paging(struct proc *caller)
{
	/* Paging already enabled during pre_init */
	return OK;
}

void release_address_space(struct proc *pr)
{
	pr->p_seg.p_satp = 0;
	pr->p_seg.p_satp_v = NULL;
}

size_t vm_lookup_range(const struct proc *proc, vir_bytes vir, phys_bytes *phys,
    size_t bytes)
{
	/* Simplified: assume identity mapping */
	if (phys)
		*phys = vir;
	return bytes;
}

void arch_proc_init(struct proc *pr, u32_t ip_entry,
    u32_t sp, u32_t ps_str, char *name)
{
	memset(&pr->p_reg, 0, sizeof(pr->p_reg));
	pr->p_reg.pc = ip_entry;
	pr->p_reg.sp = sp;
	/* Determine user vs kernel from privilege level */
	if (iskernelp(pr)) {
		pr->p_reg.status = CPUTWO_STATUS_SUPER | CPUTWO_STATUS_IE;
	} else {
		pr->p_reg.status = CPUTWO_STATUS_IE;
	}
}

int arch_get_params(char *params, int maxsize)
{
	/* No boot parameters on CPUTwo */
	params[0] = '\0';
	return OK;
}

void memory_init(void)
{
	/* Memory layout configured during pre_init */
}

void busy_delay_ms(int ms)
{
	/* Simple busy-wait delay */
	volatile int i;
	for (i = 0; i < ms * 1000; i++)
		;
}
