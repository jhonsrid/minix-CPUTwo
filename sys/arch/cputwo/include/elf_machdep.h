#ifndef _CPUTWO_ELF_MACHDEP_H_
#define _CPUTWO_ELF_MACHDEP_H_

#define ELF32_MACHDEP_ENDIANNESS    ELFDATA2LSB

#define ELF64_MACHDEP_ENDIANNESS    XXX  /* no 64-bit support */
#define ELF64_MACHDEP_ID_CASES      /* no 64-bit ELF machine types */

/* Use EM_NONE for now; a proper EM_CPUTWO should be coordinated with LLVM */
#ifndef EM_CPUTWO
#define EM_CPUTWO   0x4332  /* 'C' '2' - private use */
#endif

#define ELF32_MACHDEP_ID_CASES  \
		case EM_CPUTWO: \
			break;

#define ELF32_MACHDEP_ID    EM_CPUTWO

#define ARCH_ELFSIZE        32

/* Processor specific relocation types */
#define R_CPUTWO_NONE       0
#define R_CPUTWO_32         1
#define R_CPUTWO_PC32       2
#define R_CPUTWO_16         3
#define R_CPUTWO_8          4
#define R_CPUTWO_COPY       5
#define R_CPUTWO_GLOB_DAT   6
#define R_CPUTWO_JUMP_SLOT  7
#define R_CPUTWO_RELATIVE   8

#define R_TYPE(name)        __CONCAT(R_CPUTWO_,name)

#endif /* _CPUTWO_ELF_MACHDEP_H_ */
