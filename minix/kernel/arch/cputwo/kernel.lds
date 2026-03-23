/* CPUTwo kernel linker script - simplified for identity-mapped kernel */
OUTPUT_ARCH("cputwo")
ENTRY(MINIX)

_kern_phys_base = 0x00000000;
_kern_vir_base = 0x00000000;
_kern_offset = 0;

/* Aliases for unpaged code that references these */
__k_unpaged__kern_offset = _kern_offset;
__k_unpaged__kern_vir_base = _kern_vir_base;
__k_unpaged__kern_phys_base = _kern_phys_base;

SECTIONS
{
	. = _kern_phys_base;
	__k_unpaged__kern_unpaged_start = .;

	. = ALIGN(4096); usermapped_start = .;
	.usermapped_glo : { *(.usermapped_glo) }
	. = ALIGN(4096); usermapped_nonglo_start = .;
	.usermapped : { *(.usermapped) }
	. = ALIGN(4096); usermapped_end = .;

	.text : { *(.text*) }
	_etext = .;

	.data ALIGN(4096) : { *(.data* .rodata*) }
	. = ALIGN(4096);
	_edata = .;
	__k_unpaged__edata = .;
	__k_unpaged__kern_unpaged_edata = .;
	__k_unpaged__kern_unpaged_end = .;

	.bss ALIGN(4096) : {
		__bss_start = .;
		*(.bss* COMMON)
		__k_unpaged__kern_size = . - _kern_vir_base;
		_kern_size = __k_unpaged__kern_size;
		. += 4096;
	}
	_end = .;
	__k_unpaged__end = .;
}
