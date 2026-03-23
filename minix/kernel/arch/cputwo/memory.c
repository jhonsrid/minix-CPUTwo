/* CPUTwo memory management */
#include "kernel/kernel.h"
#include "kernel/proc.h"
#include "kernel/vm.h"
#include <machine/vm.h>
#include <minix/type.h>
#include <string.h>
#include <assert.h>
#include "archconst.h"
#include "arch_proto.h"

/* Page table root for kernel identity mapping (physical address) */
static u32_t *kernel_pt_root;

phys_bytes vir2phys(void *vir)
{
	return (phys_bytes)vir;
}

/* Create a PTE mapping for a virtual-to-physical page */
static void map_page(u32_t *pt_root, vir_bytes vaddr, phys_bytes paddr, u32_t flags)
{
	int l1_idx = (vaddr >> 22) & 0x3FF;
	int l2_idx = (vaddr >> 12) & 0x3FF;
	u32_t *l1 = pt_root;
	u32_t l1e = l1[l1_idx];
	u32_t *l2;

	if (!(l1e & CPUTWO_VM_PTE_V)) {
		panic("map_page: L1 entry not present for vaddr 0x%08lx", vaddr);
	}

	l2 = (u32_t *)((l1e >> 12) << 12);
	l2[l2_idx] = ((paddr >> 12) << 12) | flags | CPUTWO_VM_PTE_V;
}

int vm_contiguous(const struct proc *targetproc, vir_bytes vir_buf, size_t bytes)
{
	/* Simplified: assume contiguous for now */
	return 1;
}

int vm_lookup(const struct proc *proc, vir_bytes virtual, phys_bytes *physical,
    u32_t *ptent)
{
	u32_t *pt_root;
	int l1_idx, l2_idx;
	u32_t l1e, l2e;

	if (!proc || !proc->p_seg.p_satp_v)
		return EFAULT;

	pt_root = proc->p_seg.p_satp_v;
	l1_idx = (virtual >> 22) & 0x3FF;
	l2_idx = (virtual >> 12) & 0x3FF;

	l1e = pt_root[l1_idx];
	if (!(l1e & CPUTWO_VM_PTE_V))
		return EFAULT;

	u32_t *l2 = (u32_t *)((l1e >> 12) << 12);
	l2e = l2[l2_idx];
	if (!(l2e & CPUTWO_VM_PTE_V))
		return EFAULT;

	if (physical)
		*physical = ((l2e >> 12) << 12) | (virtual & 0xFFF);
	if (ptent)
		*ptent = l2e;

	return OK;
}

void __switch_address_space(struct proc *p, struct proc **ptproc)
{
	u32_t satp_val;

	if (*ptproc == p)
		return;

	/* Switch page table */
	satp_val = p->p_seg.p_satp;
	if (satp_val) {
		*(volatile u32_t *)CPUTWO_REG_SATP = satp_val;
		/* SFENCE to flush TLB */
		__asm volatile(".word 0x3E000000");  /* SFENCE opcode */
	}

	*ptproc = p;
}

/* Virtual copy between address spaces */
int virtual_copy_f(struct proc *caller, struct vir_addr *src_addr,
    struct vir_addr *dst_addr, vir_bytes bytes, int vmcheck)
{
	/* Simplified: use physical addresses when possible */
	phys_bytes phys_src, phys_dst;

	phys_src = src_addr->offset;
	phys_dst = dst_addr->offset;

	phys_copy(phys_src, phys_dst, bytes);
	return OK;
}

void vm_suspend(struct proc *caller, const struct proc *target,
    phys_bytes linaddr, phys_bytes len, int type, int writeflag)
{
	caller->p_vmrequest.req_type = type;
	caller->p_vmrequest.target = target->p_endpoint;
	caller->p_vmrequest.params.check.start = linaddr;
	caller->p_vmrequest.params.check.length = len;
	caller->p_vmrequest.params.check.writeflag = writeflag;

	RTS_SET(caller, RTS_VMREQUEST);
}

int vm_memset(struct proc *caller, endpoint_t who, phys_bytes ph,
    int c, phys_bytes count)
{
	phys_memset(ph, c, count);
	return OK;
}

int vm_check_range(struct proc *caller, struct proc *target,
    vir_bytes vir_addr, size_t bytes, int writable)
{
	return OK;
}

void mem_clear_mapcache(void)
{
	/* No map cache on CPUTwo */
}

void print_proc_recursive(struct proc *p)
{
	/* Minimal debug output */
}

int intr_init(int mine)
{
	/* Hardware interrupt initialization done in protect.c */
	return OK;
}
