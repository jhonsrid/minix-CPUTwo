#ifndef _CPUTWO_CPU_COUNTER_H_
#define _CPUTWO_CPU_COUNTER_H_

#ifdef _KERNEL
/* CPUTwo timer can be read for a counter, but no dedicated cycle counter */
#define cpu_hascounter()    false
#define cpu_counter()       cpu_counter32()

static __inline uint32_t
cpu_counter32(void)
{
	return 0;
}

static __inline uint64_t
cpu_frequency(struct cpu_info *ci)
{
	return 0;
}
#endif /* _KERNEL */

#endif /* _CPUTWO_CPU_COUNTER_H_ */
