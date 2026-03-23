#ifndef _CPUTWO_TYPES_H_
#define _CPUTWO_TYPES_H_

#include <sys/cdefs.h>
#include <sys/featuretest.h>
#include <cputwo/int_types.h>

#if defined(_KERNEL)
typedef struct label_t {
        int val[11];
} label_t;
#endif

#if defined(_NETBSD_SOURCE)
typedef unsigned long   vm_offset_t;
typedef unsigned long   vm_size_t;

typedef unsigned long   paddr_t;
typedef unsigned long   psize_t;
typedef unsigned long   vaddr_t;
typedef unsigned long   vsize_t;
#define PRIxPADDR       "lx"
#define PRIxPSIZE       "lx"
#define PRIuPSIZE       "lu"
#define PRIxVADDR       "lx"
#define PRIxVSIZE       "lx"
#define PRIuVSIZE       "lu"
#endif

typedef int             register_t, register32_t;
#define PRIxREGISTER    "x"

typedef unsigned long   pmc_evid_t;
#define PMC_INVALID_EVID    (-1)
typedef unsigned long   pmc_ctr_t;
typedef unsigned short  tlb_asid_t;

#if defined(_KERNEL)
typedef volatile unsigned char  __cpu_simple_lock_t;
#else
typedef volatile int            __cpu_simple_lock_t;
#endif

#define __SIMPLELOCK_LOCKED     1
#define __SIMPLELOCK_UNLOCKED   0

#define __HAVE_SYSCALL_INTERN
#define __HAVE_NEW_STYLE_BUS_H
#define __HAVE_MINIMAL_EMUL
#define __HAVE_CPU_DATA_FIRST
#define __HAVE_OLD_DISKLABEL

#if defined(_KERNEL)
#define __HAVE_RAS
#endif

#endif /* _CPUTWO_TYPES_H_ */
