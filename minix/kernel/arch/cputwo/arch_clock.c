/* CPUTwo clock/timer support */
#include "kernel/kernel.h"
#include "kernel/clock.h"
#include "kernel/interrupt.h"
#include <minix/minlib.h>
#include <minix/board.h>
#include "archconst.h"

static u64_t tsc_per_ms;

int init_local_timer(unsigned freq)
{
	/* Configure timer period.
	 * Timer counts in "instructions executed" units.
	 * Set a reasonable period for the given frequency.
	 * freq is in Hz. We'll use period = 100000 (~10ms at emulator speed).
	 */
	volatile u32_t *period = (volatile u32_t *)CPUTWO_TIMER_PERIOD;
	volatile u32_t *ctrl = (volatile u32_t *)CPUTWO_TIMER_CTRL;

	*period = 100000;
	*ctrl = 0x03;	/* enable + IRQ enable */

	/* Enable timer interrupt in IC */
	volatile u32_t *ic_enable = (volatile u32_t *)CPUTWO_IC_ENABLE;
	*ic_enable |= CPUTWO_IC_BIT_TIMER;

	tsc_per_ms = 100;	/* rough estimate */
	return 0;
}

void stop_local_timer(void)
{
	*(volatile u32_t *)CPUTWO_TIMER_CTRL = 0;
}

void restart_local_timer(void)
{
	*(volatile u32_t *)CPUTWO_TIMER_CTRL = 0x03;
}

static irq_hook_t cputwo_timer_hook;

int register_local_timer_handler(const irq_handler_t handler)
{
	put_irq_handler(&cputwo_timer_hook, CPUTWO_IRQ_TIMER, handler);
	return 0;
}

u64_t ms_2_cpu_time(unsigned ms)
{
	return (u64_t)ms * tsc_per_ms;
}

unsigned cpu_time_2_ms(u64_t cpu_time)
{
	if (tsc_per_ms == 0) return 0;
	return (unsigned)(cpu_time / tsc_per_ms);
}

void context_stop(struct proc *p)
{
	u64_t now = 0;	/* CPUTwo has no cycle counter; use tick-based accounting */
	p->p_cycles = p->p_cycles + 1;

	if (priv(p)->s_flags & BILLABLE)
		get_cpulocal_var(bill_ptr) = p;
}

void context_stop_idle(void)
{
	/* Nothing to do for idle accounting without a cycle counter */
}

void arch_timer_int_handler(void)
{
	/* Called from the generic timer interrupt path */
}
