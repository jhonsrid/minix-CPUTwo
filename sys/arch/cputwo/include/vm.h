#ifndef __SYS_VM_CPUTWO_H__
#define __SYS_VM_CPUTWO_H__
/*
 * CPUTwo virtual memory definitions - Sv32 compatible
 */

#define CPUTWO_PAGE_SIZE        4096
#define CPUTWO_SUPERPAGE_SIZE   (4 * 1024 * 1024)  /* 4 MB superpage */

/* Page table entry bits (Sv32 format) */
#define CPUTWO_VM_PTE_V         (1 << 0)   /* Valid */
#define CPUTWO_VM_PTE_D         (1 << 1)   /* Dirty (set by hardware on write) */
#define CPUTWO_VM_PTE_R         (1 << 5)   /* Readable */
#define CPUTWO_VM_PTE_W         (1 << 6)   /* Writable */
#define CPUTWO_VM_PTE_X         (1 << 7)   /* Executable */
#define CPUTWO_VM_PTE_U         (1 << 8)   /* User-accessible */
#define CPUTWO_VM_PTE_G         (1 << 9)   /* Global (not flushed by SFENCE) */

/* Convenience combinations */
#define CPUTWO_VM_PTE_PRESENT   CPUTWO_VM_PTE_V
#define CPUTWO_VM_PTE_SUPER     (CPUTWO_VM_PTE_R | CPUTWO_VM_PTE_W | CPUTWO_VM_PTE_X)
#define CPUTWO_VM_PTE_USER      (CPUTWO_VM_PTE_SUPER | CPUTWO_VM_PTE_U)

/* Page table structure */
#define CPUTWO_VM_PT_ENT_SIZE   4       /* Size of a page table entry */
#define CPUTWO_VM_DIR_ENT_SIZE  4       /* Size of a page dir entry */
#define CPUTWO_VM_DIR_ENTRIES   1024    /* Entries in L1 page table */
#define CPUTWO_VM_PT_ENTRIES    1024    /* Entries in L2 page table */

/* Address field shifts */
#define CPUTWO_VM_DIR_ENT_SHIFT 22      /* L1 index: bits [31:22] */
#define CPUTWO_VM_PT_ENT_SHIFT  12      /* L2 index: bits [21:12] */
#define CPUTWO_VM_PT_ENT_MASK   0x3FF   /* 10-bit mask */

/* Page table macros */
#define CPUTWO_VM_PDE(v)    (((v) >> CPUTWO_VM_DIR_ENT_SHIFT) & CPUTWO_VM_PT_ENT_MASK)
#define CPUTWO_VM_PTE(v)    (((v) >> CPUTWO_VM_PT_ENT_SHIFT) & CPUTWO_VM_PT_ENT_MASK)
#define CPUTWO_VM_PFA(e)    ((e) & 0xFFFFF000)    /* Extract physical address from PTE */

/* Address masks */
#define CPUTWO_VM_ADDR_MASK     0xFFFFF000
#define CPUTWO_VM_OFFSET_MASK   0x00000FFF

/* Table sizes */
#define CPUTWO_VM_DIR_SIZE      (CPUTWO_VM_DIR_ENTRIES * CPUTWO_VM_DIR_ENT_SIZE)
#define CPUTWO_VM_PT_SIZE       (CPUTWO_VM_PT_ENTRIES * CPUTWO_VM_PT_ENT_SIZE)
#define CPUTWO_PAGEDIR_SIZE     CPUTWO_VM_DIR_SIZE
#define CPUTWO_PAGETABLE_SIZE   CPUTWO_VM_PT_SIZE

/* SATP register */
#define CPUTWO_SATP_EN          (1u << 31)  /* MMU enable bit */
#define CPUTWO_SATP_PPN_MASK    0x000FFFFF  /* PPN field [19:0] */

/* MMIO region: always bypasses MMU when VA >= 0x03F00000 */
#define CPUTWO_MMIO_BASE        0x03F00000

#ifndef __ASSEMBLY__

#include <minix/type.h>

struct vm_ep_data {
	struct mem_map  *mem_map;
	vir_bytes       data_seg_limit;
};
#endif

#endif /* __SYS_VM_CPUTWO_H__ */
