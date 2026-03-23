/* Direct serial output for early boot / panic on CPUTwo */
#include "kernel/kernel.h"
#include "archconst.h"
#include "direct_utils.h"

void direct_cls(void)
{
	/* Serial console: no screen to clear */
}

void direct_print_char(char c)
{
	volatile u32_t *status = (volatile u32_t *)CPUTWO_UART_STATUS;
	volatile u32_t *tx = (volatile u32_t *)CPUTWO_UART_TX;

	/* Wait for TX ready */
	while (!(*status & 0x01))
		;
	*tx = (u32_t)c;
}

void direct_print(const char *str)
{
	while (*str) {
		if (*str == '\n')
			direct_print_char('\r');
		direct_print_char(*str++);
	}
}

int direct_read_char(unsigned char *c)
{
	volatile u32_t *status = (volatile u32_t *)CPUTWO_UART_STATUS;
	volatile u32_t *rx = (volatile u32_t *)CPUTWO_UART_RX;

	if (*status & 0x02) {
		*c = (unsigned char)*rx;
		return 1;
	}
	return 0;
}
