/* CPUTwo exception handling */
#include "kernel/kernel.h"
#include "archconst.h"
#include "hw_intr.h"
#include <signal.h>
#include <string.h>
#include <assert.h>
#include <machine/vm.h>

extern void * __copy_msg_from_user_end;
extern void * __copy_msg_to_user_end;

static void pagefault(struct proc *pr, reg_t addr, int is_write)
{
	/* If it was during phys_copy or phys_memset, return error */
	if (pr->p_seg.p_satp == 0) {
		/* Kernel page fault in unmapped mode - panic */
		panic("kernel page fault at 0x%08x", addr);
	}

	/* Inform VM about the page fault */
	pr->p_pagefault.pf_virtual = addr;
	pr->p_pagefault.pf_flags = is_write ? CPUTWO_VM_PTE_W : CPUTWO_VM_PTE_R;

	/* Suspend the process and notify VM */
	if (RTS_ISSET(pr, RTS_PAGEFAULT))
		panic("double pagefault on proc %d", pr->p_endpoint);

	RTS_SET(pr, RTS_PAGEFAULT);
}

void exception_handler(int cause)
{
	struct proc *saved_proc;
	reg_t epc, badaddr;

	saved_proc = get_cpulocal_var(proc_ptr);
	epc = *(volatile u32_t *)CPUTWO_REG_EPC;
	badaddr = *(volatile u32_t *)CPUTWO_REG_BADADDR;

	/* If exception happened in kernel, panic */
	if (saved_proc->p_reg.status & CPUTWO_STATUS_SUPER) {
		panic("kernel exception %d at pc=0x%08x", cause, epc);
	}

	switch (cause) {
	case CPUTWO_EXC_IPGFAULT:
		pagefault(saved_proc, badaddr, 0);
		break;
	case CPUTWO_EXC_LPGFAULT:
		pagefault(saved_proc, badaddr, 0);
		break;
	case CPUTWO_EXC_SPGFAULT:
		pagefault(saved_proc, badaddr, 1);
		break;
	case CPUTWO_EXC_ILLEGAL:
		cause_sig(proc_nr(saved_proc), SIGILL);
		break;
	case CPUTWO_EXC_BUSERR:
	case CPUTWO_EXC_MISALIGN:
		cause_sig(proc_nr(saved_proc), SIGBUS);
		break;
	case CPUTWO_EXC_DIVZERO:
		cause_sig(proc_nr(saved_proc), SIGFPE);
		break;
	default:
		panic("unexpected exception %d from proc %d at pc 0x%08x",
		    cause, saved_proc->p_endpoint, epc);
	}
}

void irq_handler(void)
{
	volatile u32_t *ic_pending = (volatile u32_t *)CPUTWO_IC_PENDING;
	u32_t pending = *ic_pending;

	if (pending & CPUTWO_IC_BIT_TIMER) {
		*(volatile u32_t *)CPUTWO_IC_ACK = CPUTWO_IC_BIT_TIMER;
		irq_handle(CPUTWO_IRQ_TIMER);
	}
	if (pending & CPUTWO_IC_BIT_UART_RX) {
		*(volatile u32_t *)CPUTWO_IC_ACK = CPUTWO_IC_BIT_UART_RX;
		irq_handle(CPUTWO_IRQ_UART_RX);
	}
	if (pending & CPUTWO_IC_BIT_UART_TX) {
		*(volatile u32_t *)CPUTWO_IC_ACK = CPUTWO_IC_BIT_UART_TX;
		irq_handle(CPUTWO_IRQ_UART_TX);
	}
	if (pending & CPUTWO_IC_BIT_BLKDEV) {
		*(volatile u32_t *)CPUTWO_IC_ACK = CPUTWO_IC_BIT_BLKDEV;
		irq_handle(CPUTWO_IRQ_BLKDEV);
	}
}

void proc_stacktrace(struct proc *whichproc)
{
	/* Minimal stack trace */
	printf("proc %d (endpoint %d) pc=0x%08x sp=0x%08x lr=0x%08x\n",
	    proc_nr(whichproc), whichproc->p_endpoint,
	    whichproc->p_reg.pc, whichproc->p_reg.sp, whichproc->p_reg.lr);
}

void enable_fpu_exception(void) { /* no FPU */ }
void disable_fpu_exception(void) { /* no FPU */ }
