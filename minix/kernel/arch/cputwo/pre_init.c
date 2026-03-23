/* CPUTwo pre-initialization before paging */
#include "kernel/kernel.h"
#include "archconst.h"
#include "arch_proto.h"
#include <string.h>
#include <machine/vm.h>

extern char _etext, _end;
extern char k_initial_stktop;

static void __attribute__((unused)) early_print(const char *s)
{
	volatile u32_t *status = (volatile u32_t *)0x03F00000;
	volatile u32_t *tx = (volatile u32_t *)0x03F00004;
	while (*s) {
		while (!(*status & 0x01))
			;
		if (*s == '\n') {
			*tx = '\r';
			while (!(*status & 0x01))
				;
		}
		*tx = (u32_t)*s++;
	}
}

/* Static kinfo that pre_init populates and passes to kmain */
static kinfo_t pre_kinfo;
static struct kmessages pre_kmess;

kinfo_t *pre_init(int dummy)
{
	/* Clear BSS */
	extern char __bss_start;
	memset(&__bss_start, 0, &_end - &__bss_start);

	/* Set up minimal kinfo */
	memset(&pre_kinfo, 0, sizeof(pre_kinfo));
	memset(&pre_kmess, 0, sizeof(pre_kmess));
	pre_kinfo.kmessages = &pre_kmess;

	/* Memory: 63 MB of RAM */
	pre_kinfo.mmap_size = 1;
	pre_kinfo.memmap[0].mm_base_addr = 0;
	pre_kinfo.memmap[0].mm_length = PHYS_MEM_END + 1;
	pre_kinfo.memmap[0].mm_type = MULTIBOOT_MEMORY_AVAILABLE;

	/* No boot modules yet (will be set up later) */
	pre_kinfo.mbi.mi_mods_count = 0;
	pre_kinfo.mods_with_kernel = 0;
	pre_kinfo.kern_mod = 0;

	/* Kernel location */
	pre_kinfo.vir_kern_start = (vir_bytes)&_etext;
	pre_kinfo.bootstrap_start = 0;
	pre_kinfo.bootstrap_len = 0;

	/* Release/version */
	strlcpy(pre_kinfo.release, "3.3.0", sizeof(pre_kinfo.release));
	strlcpy(pre_kinfo.version, "0", sizeof(pre_kinfo.version));

	pre_kinfo.nr_procs = NR_BOOT_PROCS - NR_TASKS;
	pre_kinfo.nr_tasks = NR_TASKS;

	return &pre_kinfo;
}

/* Stubs for missing library functions */

/* Match libkern.h: void kern_assert(const char *, ...) */
#include <stdarg.h>
void kern_assert(const char *fmt, ...)
{
	panic("kernel assertion failed: %s", fmt);
}

/* 64-bit division helper for CPUTwo (no hardware 64-bit divide) */
unsigned long long __udivdi3(unsigned long long num, unsigned long long den)
{
	unsigned long long quot = 0, qbit = 1;
	if (den == 0) return 0;
	while ((long long)den >= 0) {
		den <<= 1;
		qbit <<= 1;
	}
	while (qbit) {
		if (den <= num) {
			num -= den;
			quot += qbit;
		}
		den >>= 1;
		qbit >>= 1;
	}
	return quot;
}

unsigned long long __umoddi3(unsigned long long num, unsigned long long den)
{
	return num - __udivdi3(num, den) * den;
}

/* delivermsg - called from proc.c to deliver pending message to process */
void delivermsg(struct proc *rp)
{
	copy_msg_to_user(&rp->p_delivermsg, (message *)rp->p_delivermsg_vir);
	rp->p_delivermsg.m_source = NONE;
	rp->p_misc_flags &= ~MF_DELIVERMSG;
}

/* read_tsc_64 - read a 64-bit timestamp counter (stub for CPUTwo) */
void read_tsc_64(u64_t *t)
{
	*t = 0;  /* CPUTwo has no TSC */
}

/* data_copy - copy data between address spaces */
int data_copy(endpoint_t from_e, vir_bytes from_addr,
    endpoint_t to_e, vir_bytes to_addr, size_t len)
{
	/* Simplified: direct memory copy (valid for identity-mapped kernel) */
	memcpy((void *)to_addr, (void *)from_addr, len);
	return OK;
}

/* data_copy_vmcheck - copy with VM fault checking */
int data_copy_vmcheck(struct proc *caller,
    endpoint_t from_e, vir_bytes from_addr,
    endpoint_t to_e, vir_bytes to_addr, size_t len)
{
	memcpy((void *)to_addr, (void *)from_addr, len);
	return OK;
}

/* Port I/O stubs — CPUTwo uses MMIO, not port I/O */
#include <minix/portio.h>
unsigned inb(u16_t port) { return 0; }
unsigned inw(u16_t port) { return 0; }
unsigned inl(u16_t port) { return 0; }
void outb(u16_t port, u8_t val) { }
void outw(u16_t port, u16_t val) { }
void outl(u16_t port, u32_t val) { }

/* NetBSD assert function */
void __assert13(const char *file, int line, const char *func, const char *expr)
{
	panic("assert \"%s\" failed: file %s, line %d, function %s",
	    expr, file, line, func ? func : "?");
}

/* BSD strlcpy */
size_t strlcpy(char *dst, const char *src, size_t siz)
{
	char *d = dst;
	const char *s = src;
	size_t n = siz;

	if (n != 0) {
		while (--n != 0) {
			if ((*d++ = *s++) == '\0')
				break;
		}
	}
	if (n == 0) {
		if (siz != 0)
			*d = '\0';
		while (*s++)
			;
	}
	return (s - src - 1);
}
