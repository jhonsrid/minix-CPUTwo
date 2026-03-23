// get_bp.S - return frame pointer (r11) in r0
#include <machine/asm.h>

ENTRY(get_bp)
	mov	r0, r11
	mov	r15, r14
