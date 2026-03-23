/* CPUTwo system reset / shutdown */
#include "kernel/kernel.h"
#include "archconst.h"
#include "arch_proto.h"
#include "direct_utils.h"
#include <sys/reboot.h>

void halt_cpu(void)
{
	/* Halt: disable interrupts and spin */
	*(volatile u32_t *)CPUTWO_REG_STATUS = CPUTWO_STATUS_SUPER;
	for (;;)
		;
}

__dead void reset(void)
{
	direct_print("System reset - halting\n");
	halt_cpu();
	for (;;) ;
}

__dead void poweroff(void)
{
	direct_print("Power off - halting\n");
	halt_cpu();
	for (;;) ;
}

__dead void arch_shutdown(int how)
{
	direct_print("\nMinix shutting down...\n");

	if ((how & RB_POWERDOWN) == RB_POWERDOWN) {
		poweroff();
		NOT_REACHABLE;
	}

	if (how & RB_HALT) {
		for (;;) halt_cpu();
		NOT_REACHABLE;
	}

	reset();
	NOT_REACHABLE;

	while (1);
}
