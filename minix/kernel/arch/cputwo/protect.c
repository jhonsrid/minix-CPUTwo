/* CPUTwo protection and boot initialization */
#include "kernel/kernel.h"
#include "kernel/proc.h"
#include "archconst.h"
#include "arch_proto.h"
#include <machine/vm.h>
#include <string.h>
#include <assert.h>
#include <minix/type.h>
extern u32_t cputwo_exc_table[];

void prot_init(void)
{
	/* Set up exception vector table */
	*(volatile u32_t *)CPUTWO_REG_EVEC = (u32_t)cputwo_exc_table;
}

int tss_init(unsigned cpu, void *kernel_stack)
{
	/* On CPUTwo, store the kernel stack pointer for this CPU.
	 * Used during trap entry to switch to kernel stack.
	 */
	return 0;
}

void arch_post_init(void)
{
	/* Nothing extra needed after VM is running */
}

void arch_boot_proc(struct boot_image *ip, struct proc *rp)
{
	/* Load a boot image process into its address space */
	rp->p_reg.pc = ip->start_addr;
}

void arch_post_romboot(void)
{
	/* Nothing to do */
}

multiboot_module_t *bootmod(int pnr)
{
	return NULL;
}
