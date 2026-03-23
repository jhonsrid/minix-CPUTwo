#ifndef _MACHINE_ELF_H_
#define _MACHINE_ELF_H_ 1

/*
 * ELF definitions for the CPUTwo architecture.
 */

#ifndef EM_CPUTWO
#define EM_CPUTWO   0x4332
#endif

#define ELF_TARG_CLASS  ELFCLASS32
#define ELF_TARG_DATA   ELFDATA2LSB
#define ELF_TARG_MACH   EM_CPUTWO
#define ELF_TARG_VER    1

#endif /* !_MACHINE_ELF_H_ */
