#ifndef _CPUTWO_LOCK_H_
#define _CPUTWO_LOCK_H_

static __inline int
__SIMPLELOCK_LOCKED_P(__cpu_simple_lock_t *__ptr)
{
	return *__ptr == __SIMPLELOCK_LOCKED;
}

static __inline int
__SIMPLELOCK_UNLOCKED_P(__cpu_simple_lock_t *__ptr)
{
	return *__ptr == __SIMPLELOCK_UNLOCKED;
}

static __inline void
__cpu_simple_lock_clear(__cpu_simple_lock_t *__ptr)
{
	*__ptr = __SIMPLELOCK_UNLOCKED;
}

static __inline void
__cpu_simple_lock_set(__cpu_simple_lock_t *__ptr)
{
	*__ptr = __SIMPLELOCK_LOCKED;
}

static __inline void __unused
__cpu_simple_lock_init(__cpu_simple_lock_t *alp)
{
	*alp = __SIMPLELOCK_UNLOCKED;
}

static __inline void __unused
__cpu_simple_lock(__cpu_simple_lock_t *alp)
{
	while (__sync_lock_test_and_set(alp, __SIMPLELOCK_LOCKED) != __SIMPLELOCK_UNLOCKED)
		continue;
}

static __inline int __unused
__cpu_simple_lock_try(__cpu_simple_lock_t *alp)
{
	return (__sync_lock_test_and_set(alp, __SIMPLELOCK_LOCKED) == __SIMPLELOCK_UNLOCKED);
}

static __inline void __unused
__cpu_simple_unlock(__cpu_simple_lock_t *alp)
{
	__sync_lock_release(alp);
}

#endif /* _CPUTWO_LOCK_H_ */
