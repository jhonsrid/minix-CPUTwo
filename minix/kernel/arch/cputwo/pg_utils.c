/* CPUTwo page table utilities for Sv32 */
#include "kernel/kernel.h"
#include "archconst.h"
#include "arch_proto.h"
#include <machine/vm.h>
#include <string.h>
#include <assert.h>

static phys_bytes freepg_start;
static phys_bytes freepg_end;
static phys_bytes kern_pt_root;	/* physical address of L1 page table */

void add_memmap(kinfo_t *cbi, u64_t addr, u64_t len)
{
	int i;
	for (i = 0; i < cbi->mmap_size; i++) {
		if (cbi->memmap[i].mm_base_addr == 0 && cbi->memmap[i].mm_length == 0) {
			cbi->memmap[i].mm_base_addr = addr;
			cbi->memmap[i].mm_length = len;
			cbi->memmap[i].mm_type = MULTIBOOT_MEMORY_AVAILABLE;
			if (i + 1 > cbi->mmap_size)
				cbi->mmap_size = i + 1;
			return;
		}
	}
}

void cut_memmap(kinfo_t *cbi, phys_bytes start, phys_bytes end)
{
	/* Remove a range from the memory map */
	/* Simplified implementation */
}

phys_bytes pg_roundup(phys_bytes b)
{
	return (b + CPUTWO_PAGE_SIZE - 1) & ~(CPUTWO_PAGE_SIZE - 1);
}

static phys_bytes pg_alloc_page(void)
{
	phys_bytes page = freepg_start;
	if (freepg_start >= freepg_end)
		panic("pg_alloc_page: out of memory");
	freepg_start += CPUTWO_PAGE_SIZE;
	memset((void *)page, 0, CPUTWO_PAGE_SIZE);
	return page;
}

void pg_info(reg_t *satp_val, u32_t **pt_root)
{
	*satp_val = *(volatile u32_t *)CPUTWO_REG_SATP;
	*pt_root = (u32_t *)((*satp_val & CPUTWO_SATP_PPN_MASK) << 12);
}

void pg_clear(void)
{
	/* Disable MMU */
	*(volatile u32_t *)CPUTWO_REG_SATP = 0;
}

void pg_identity(kinfo_t *cbi)
{
	u32_t *l1;
	int i;
	phys_bytes addr;

	/* Set up free page allocator from after kernel end */
	extern char _end;
	freepg_start = pg_roundup((phys_bytes)&_end);
	freepg_end = PHYS_MEM_END - 0x10000;

	/* Allocate L1 page table */
	l1 = (u32_t *)pg_alloc_page();

	/* Identity map all RAM using L2 tables (4KB pages) */
	for (addr = 0; addr < (phys_bytes)(PHYS_MEM_END + 1); addr += (4 * 1024 * 1024)) {
		u32_t *l2 = (u32_t *)pg_alloc_page();
		int l1_idx = (addr >> 22) & 0x3FF;

		/* Fill L2 entries */
		for (i = 0; i < 1024 && (addr + i * CPUTWO_PAGE_SIZE) <= PHYS_MEM_END; i++) {
			phys_bytes pa = addr + i * CPUTWO_PAGE_SIZE;
			l2[i] = (pa & 0xFFFFF000) | CPUTWO_VM_PTE_V |
			    CPUTWO_VM_PTE_R | CPUTWO_VM_PTE_W | CPUTWO_VM_PTE_X |
			    CPUTWO_VM_PTE_G;
		}

		l1[l1_idx] = ((phys_bytes)l2 & 0xFFFFF000) | CPUTWO_VM_PTE_V;
	}

	/* Store L1 root for kernel use */
	kern_pt_root = (phys_bytes)l1;
}

phys_bytes pg_load(void)
{
	phys_bytes pt_root = kern_pt_root;
	u32_t satp = CPUTWO_SATP_EN | (pt_root >> 12);
	*(volatile u32_t *)CPUTWO_REG_SATP = satp;
	/* SFENCE to flush TLB */
	__asm volatile(".word 0x3E000000");
	return pt_root;
}

void pg_map(phys_bytes phys, vir_bytes vaddr, vir_bytes vaddr_end, kinfo_t *cbi)
{
	u32_t *l1 = (u32_t *)kern_pt_root;

	while (vaddr < vaddr_end) {
		int l1_idx = (vaddr >> 22) & 0x3FF;
		int l2_idx = (vaddr >> 12) & 0x3FF;
		u32_t *l2;

		if (!(l1[l1_idx] & CPUTWO_VM_PTE_V)) {
			l2 = (u32_t *)pg_alloc_page();
			l1[l1_idx] = ((phys_bytes)l2 & 0xFFFFF000) | CPUTWO_VM_PTE_V;
		} else {
			l2 = (u32_t *)((l1[l1_idx] >> 12) << 12);
		}

		l2[l2_idx] = (phys & 0xFFFFF000) | CPUTWO_VM_PTE_V |
		    CPUTWO_VM_PTE_R | CPUTWO_VM_PTE_W | CPUTWO_VM_PTE_X |
		    CPUTWO_VM_PTE_G;

		vaddr += CPUTWO_PAGE_SIZE;
		phys += CPUTWO_PAGE_SIZE;
	}
}

int pg_mapkernel(void)
{
	/* Kernel is identity-mapped; nothing extra needed */
	return OK;
}

void pg_mapproc(struct proc *p, struct boot_image *ip, kinfo_t *cbi)
{
	/* Map process memory - done by VM server later */
}

phys_bytes alloc_lowest(kinfo_t *cbi, phys_bytes len)
{
	phys_bytes ret = pg_roundup(freepg_start);
	freepg_start = ret + len;
	return ret;
}

void vm_enable_paging(void)
{
	/* Enable MMU by loading kernel page table into SATP */
	pg_load();
}
