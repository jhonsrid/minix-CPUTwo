#include <sys/types.h>
#include <minix/u64.h>
#include <minix/config.h>
#include <minix/const.h>
#include <minix/minlib.h>
#include <machine/archtypes.h>

#ifndef CONFIG_MAX_CPUS
#define CONFIG_MAX_CPUS 1
#endif

#define MICROHZ		1000000

/* CPUTwo has no TSC; these are stubs that return 0 */

u32_t tsc_64_to_micros(u64_t tsc)
{
	return (u32_t)tsc;
}

u32_t tsc_to_micros(u32_t low, u32_t high)
{
	return tsc_64_to_micros(make64(low, high));
}
