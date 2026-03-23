/* Hardware interrupt controller interface for CPUTwo */
#include "hw_intr.h"
#include "archconst.h"

int hw_intr_mask(int irq)
{
	volatile u32_t *enable = (volatile u32_t *)CPUTWO_IC_ENABLE;
	*enable &= ~(1u << irq);
	return 0;
}

int hw_intr_unmask(int irq)
{
	volatile u32_t *enable = (volatile u32_t *)CPUTWO_IC_ENABLE;
	*enable |= (1u << irq);
	return 0;
}

int hw_intr_ack(int irq)
{
	volatile u32_t *ack = (volatile u32_t *)CPUTWO_IC_ACK;
	*ack = (1u << irq);
	return 0;
}

int hw_intr_used(int irq) { return 0; }
int hw_intr_not_used(int irq) { return 0; }
int hw_intr_disable_all(void)
{
	*(volatile u32_t *)CPUTWO_IC_ENABLE = 0;
	return 0;
}
