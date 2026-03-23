#ifndef _CPUTWO_PCB_H_
#define _CPUTWO_PCB_H_

#include <machine/frame.h>

struct pcb {
	/* Callee-saved registers for kernel context switch */
	unsigned int pcb_r4;
	unsigned int pcb_r5;
	unsigned int pcb_r6;
	unsigned int pcb_r7;
	unsigned int pcb_r8;
	unsigned int pcb_r9;
	unsigned int pcb_r10;
	unsigned int pcb_r11;
	unsigned int pcb_sp;
	unsigned int pcb_lr;
	unsigned int pcb_satp;  /* page table root */
};

#endif /* _CPUTWO_PCB_H_ */
