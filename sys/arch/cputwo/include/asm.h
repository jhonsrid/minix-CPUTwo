#ifndef _CPUTWO_ASM_H_
#define _CPUTWO_ASM_H_

#define _C_LABEL(x)     x
#define _ASM_LABEL(x)   x

#define _ENTRY(x) \
	.text; .globl x; .type x,%function; x:

#define ENTRY(y)    _ENTRY(_C_LABEL(y))
#define ASENTRY(y)  _ENTRY(_ASM_LABEL(y))
#define END(y)      .size y, . - y

#define STRONG_ALIAS(alias,sym)                     \
	.globl alias;                                   \
	alias = sym
#define WEAK_ALIAS(alias,sym)                       \
	.weak alias;                                    \
	alias = sym

#ifdef __STDC__
#define __CONCAT(x,y)   x ## y
#define __STRING(x)     #x
#else
#define __CONCAT(x,y)   x/**/y
#define __STRING(x)     "x"
#endif

#endif /* _CPUTWO_ASM_H_ */
