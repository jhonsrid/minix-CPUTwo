#include <minix/minlib.h>
#include <minix/type.h>

void read_tsc(u32_t *hi, u32_t *lo)
{
	/* CPUTwo has no TSC; return 0 */
	*hi = 0;
	*lo = 0;
}

void read_tsc_64(u64_t *t)
{
	*t = 0;
}
