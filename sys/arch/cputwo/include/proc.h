#ifndef _CPUTWO_PROC_H_
#define _CPUTWO_PROC_H_

struct mdlwp {
	struct trapframe *md_tf;
};

struct mdproc {
	void (*md_syscall)(struct trapframe *);
	int md_flags;
};

#endif /* _CPUTWO_PROC_H_ */
