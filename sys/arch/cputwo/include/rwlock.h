#ifndef _CPUTWO_RWLOCK_H_
#define _CPUTWO_RWLOCK_H_

struct krwlock {
	volatile uintptr_t	rw_owner;
};

#ifdef __RWLOCK_PRIVATE

#define __HAVE_SIMPLE_RW_LOCKS  1

#define RW_RECEIVE(rw)          /* nothing */
#define RW_GIVE(rw)             /* nothing */

unsigned long _lock_cas(volatile unsigned long *,
    unsigned long, unsigned long);

#define RW_CAS(p, o, n)         \
    (_lock_cas((volatile unsigned long *)(p), (o), (n)) == (o))

#endif /* __RWLOCK_PRIVATE */

#endif /* _CPUTWO_RWLOCK_H_ */
