#ifndef _CPUTWO_PARAM_H_
#define _CPUTWO_PARAM_H_

#if defined(_KERNEL)
# define _MACHINE_ARCH  cputwo
# define MACHINE_ARCH   "cputwo"
# define _MACHINE       cputwo
# define MACHINE        "cputwo"
#else
# undef _MACHINE
# undef MACHINE
# undef _MACHINE_ARCH
# undef MACHINE_ARCH
# define _MACHINE       cputwo
# define MACHINE        "cputwo"
# define _MACHINE_ARCH  cputwo
# define MACHINE_ARCH   "cputwo"
#endif

#define MID_MACHINE     MID_CPUTWO

/* Stack alignment: 8 bytes */
#define STACK_ALIGNBYTES    (8 - 1)
#define ALIGNBYTES32        3

#define DEV_BSHIFT      9
#define DEV_BSIZE       (1 << DEV_BSHIFT)
#define BLKDEV_IOSIZE   2048

#ifndef MAXPHYS
#define MAXPHYS         65536
#endif

#define MSIZE           256

#ifndef MCLSHIFT
#define MCLSHIFT        11
#endif

#define MCLBYTES        (1 << MCLSHIFT)

#ifndef NMBCLUSTERS_MAX
#define NMBCLUSTERS_MAX (0x2000000 / MCLBYTES)
#endif

/* Page size */
#define PGSHIFT         12
#define NBPG            (1 << PGSHIFT)
#define PGOFSET         (NBPG - 1)
#define NPTEPG          (NBPG / sizeof(void *))

#define SSIZE           1
#define SINCR           1
#define UPAGES          2
#define USPACE          (UPAGES * NBPG)

#endif /* _CPUTWO_PARAM_H_ */
