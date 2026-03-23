/* Forward to Minix kernel archconst.h via include path */
#ifndef _CPUTWO_MACHINE_ARCHCONST_H_
#define _CPUTWO_MACHINE_ARCHCONST_H_

/* This is the machine/ archconst - just include the Minix arch one.
 * The Minix-specific archconst.h is at minix/include/arch/cputwo/include/archconst.h
 * and should be found via the include path. If not, define the minimal
 * constant needed by minix/const.h here. */
#include <minix/sys_config.h>

#ifndef DEFAULT_HZ
#define DEFAULT_HZ 1000
#endif

#endif /* _CPUTWO_MACHINE_ARCHCONST_H_ */
