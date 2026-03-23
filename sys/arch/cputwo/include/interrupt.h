/* Interrupt numbers and hardware vectors for CPUTwo */

#ifndef _CPUTWO_INTERRUPT_H
#define _CPUTWO_INTERRUPT_H

/* CPUTwo has 4 interrupt sources via the Interrupt Controller */
#define NR_IRQ_VECTORS    4

/* IRQ assignments */
#define CPUTWO_IRQ_TIMER      0
#define CPUTWO_IRQ_UART_RX    1
#define CPUTWO_IRQ_UART_TX    2
#define CPUTWO_IRQ_BLKDEV     3

#endif /* _CPUTWO_INTERRUPT_H */
