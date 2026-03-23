#ifndef _CPUTWO_MUTEX_H_
#define _CPUTWO_MUTEX_H_

#ifndef __MUTEX_PRIVATE

struct kmutex {
	uintptr_t	mtx_pad1;
};

#else

struct kmutex {
	union {
		volatile uintptr_t	mtxa_owner;
		struct {
			volatile uint8_t	mtxs_dummy;
			ipl_cookie_t		mtxs_ipl;
			__cpu_simple_lock_t	mtxs_lock;
			volatile uint8_t	mtxs_unused;
		} s;
	} u;
};

#define mtx_owner   u.mtxa_owner
#define mtx_ipl     u.s.mtxs_ipl
#define mtx_lock    u.s.mtxs_lock

#define __HAVE_SIMPLE_MUTEXES   1

#define MUTEX_RECEIVE(mtx)      /* nothing */
#define MUTEX_GIVE(mtx)         /* nothing */

#define MUTEX_CAS(p, o, n)      \
    (atomic_cas_ulong((volatile unsigned long *)(p), (o), (n)) == (o))

#endif /* __MUTEX_PRIVATE */

#endif /* _CPUTWO_MUTEX_H_ */
