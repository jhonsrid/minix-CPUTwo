#ifndef _CPUTWO_ACONST_H
#define _CPUTWO_ACONST_H

#include <machine/interrupt.h>
#include <machine/memory.h>
#include <machine/cpu.h>

/*
 * CPUTwo STATUS register bits
 */
#define CPUTWO_STATUS_SUPER	0x01	/* Supervisor mode */
#define CPUTWO_STATUS_IE	0x02	/* Interrupt enable */

/* Initial STATUS for user processes: user mode, interrupts enabled */
#define INIT_PSR	CPUTWO_STATUS_IE
/* Initial STATUS for kernel tasks: supervisor mode, interrupts enabled */
#define INIT_TASK_PSR	(CPUTWO_STATUS_SUPER | CPUTWO_STATUS_IE)

/* Exception cause codes (from CPUTwo EVEC table) */
#define CPUTWO_EXC_ILLEGAL	0x00
#define CPUTWO_EXC_MISALIGN	0x01
#define CPUTWO_EXC_BUSERR	0x02
#define CPUTWO_EXC_SYSCALL	0x03
#define CPUTWO_EXC_DIVZERO	0x04
#define CPUTWO_EXC_HALT		0x05
#define CPUTWO_EXC_IRQ		0x06
#define CPUTWO_EXC_IPGFAULT	0x07
#define CPUTWO_EXC_LPGFAULT	0x08
#define CPUTWO_EXC_SPGFAULT	0x09
#define CPUTWO_NR_EXCEPTIONS	10

/*
 * CPUTwo supervisor registers (memory-mapped)
 */
#define CPUTWO_REG_EPC		0x03FFF000
#define CPUTWO_REG_EFLAGS	0x03FFF004
#define CPUTWO_REG_EVEC		0x03FFF008
#define CPUTWO_REG_CAUSE	0x03FFF00C
#define CPUTWO_REG_STATUS	0x03FFF010
#define CPUTWO_REG_ESTATUS	0x03FFF014
#define CPUTWO_REG_SATP		0x03FFF018
#define CPUTWO_REG_BADADDR	0x03FFF01C
#define CPUTWO_REG_WFI		0x03FFF020

/*
 * CPUTwo device MMIO addresses
 */
#define CPUTWO_UART_BASE	0x03F00000
#define CPUTWO_UART_STATUS	0x03F00000
#define CPUTWO_UART_TX		0x03F00004
#define CPUTWO_UART_RX		0x03F00008
#define CPUTWO_UART_CTRL	0x03F0000C

#define CPUTWO_TIMER_BASE	0x03F01000
#define CPUTWO_TIMER_PERIOD	0x03F01000
#define CPUTWO_TIMER_CTRL	0x03F01004

#define CPUTWO_IC_BASE		0x03F02000
#define CPUTWO_IC_PENDING	0x03F02000
#define CPUTWO_IC_ENABLE	0x03F02004
#define CPUTWO_IC_ACK		0x03F02008

#define CPUTWO_IC_BIT_TIMER	(1 << 0)
#define CPUTWO_IC_BIT_UART_RX	(1 << 1)
#define CPUTWO_IC_BIT_UART_TX	(1 << 2)
#define CPUTWO_IC_BIT_BLKDEV	(1 << 3)

/*
 * Reserved space at top of kernel stack for cpulocal data
 */
#define CPUTWO_STACK_TOP_RESERVED	(2 * sizeof(reg_t))

/*
 * User-changeable PSR bits: only the IE bit (bit 1)
 * Supervisor bit (bit 0) is protected
 */
#define SET_USR_PSR(rp, npsr) \
	rp->p_reg.status = (rp->p_reg.status & 0x01) | (npsr & ~0x01)

#define PG_ALLOCATEME ((phys_bytes)-1)

#endif /* _CPUTWO_ACONST_H */
