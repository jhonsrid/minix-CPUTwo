#ifndef _CPUTWO_PROFILE_H_
#define _CPUTWO_PROFILE_H_

#define _MCOUNT_DECL void _mcount

/* Profiling not yet supported on CPUTwo */
#define MCOUNT  /* nothing */

#ifdef _KERNEL
#define MCOUNT_ENTER    /* nothing */
#define MCOUNT_EXIT     /* nothing */
#endif

#endif /* _CPUTWO_PROFILE_H_ */
