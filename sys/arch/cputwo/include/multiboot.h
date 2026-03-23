/* Minimal multiboot header for CPUTwo - QEMU loads kernel directly */
#ifndef _CPUTWO_MULTIBOOT_H_
#define _CPUTWO_MULTIBOOT_H_

#include <sys/types.h>

typedef struct multiboot_info {
	u_int32_t	mi_flags;
	u_int32_t	mi_mem_lower;
	u_int32_t	mi_mem_upper;
	u_int32_t	mi_boot_device;
	u_int32_t	mi_cmdline;
	u_int32_t	mi_mods_count;
	u_int32_t	mi_mods_addr;
	u_int32_t	mi_elfshdr_num;
	u_int32_t	mi_elfshdr_size;
	u_int32_t	mi_elfshdr_addr;
	u_int32_t	mi_elfshdr_shndx;
	u_int32_t	mi_mmap_length;
	u_int32_t	mi_mmap_addr;
} multiboot_info_t;

#define MULTIBOOT_INFO_HAS_MMAP	0x00000040

typedef struct multiboot_module {
	u_int32_t	mod_start;
	u_int32_t	mod_end;
	u_int32_t	string;
	u_int32_t	reserved;
} multiboot_module_t;

typedef struct multiboot_mmap {
	u_int32_t	mm_size;
	u_int64_t	mm_base_addr;
	u_int64_t	mm_length;
	u_int32_t	mm_type;
} multiboot_mmap_t;

#define MULTIBOOT_MEMORY_AVAILABLE	1

typedef struct multiboot_mmap multiboot_memory_map_t;

#define MULTIBOOT_MAX_MODS	20
#define MULTIBOOT_PARAM_BUF_SIZE 1024

#endif /* _CPUTWO_MULTIBOOT_H_ */
