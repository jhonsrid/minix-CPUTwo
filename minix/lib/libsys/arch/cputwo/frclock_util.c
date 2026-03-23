/* CPUTwo free running clock utilities.
 * CPUTwo has no user-accessible free running clock, so we use a simple
 * counter that increments on each read (for delta calculations) and
 * fall back to tickdelay for actual delays. */

#include <minix/minlib.h>
#include <minix/sysutil.h>
#include <minix/type.h>
#include <sys/errno.h>
#include <sys/types.h>

#define MICROHZ         1000000ULL

static u32_t fake_counter = 0;

int
micro_delay(u32_t micros)
{
	u64_t Hz = sys_hz();

	/* Use tickdelay for all delays since we have no hardware counter */
	if (micros > 0) {
		unsigned long ticks = (unsigned long)(micros * Hz / MICROHZ);
		if (ticks == 0) ticks = 1;
		tickdelay(ticks);
	}

	return 0;
}

u32_t frclock_64_to_micros(u64_t tsc)
{
	/* No real clock; return tsc as-is (it's a fake counter) */
	return (u32_t)tsc;
}

void
read_frclock(u32_t *frclk)
{
	/* No hardware free running clock; use a monotonic fake counter */
	*frclk = ++fake_counter;
}

u32_t
delta_frclock(u32_t base, u32_t cur)
{
	if (cur < base)
		return (UINT_MAX - base) + cur;
	return cur - base;
}

void
read_frclock_64(u64_t *frclk)
{
	u32_t val;
	read_frclock(&val);
	*frclk = (u64_t)val;
}

u64_t
delta_frclock_64(u64_t base, u64_t cur)
{
	return (u64_t)delta_frclock((u32_t)base, (u32_t)cur);
}
