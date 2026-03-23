#ifndef _CPUTWO_INT_CONST_H_
#define _CPUTWO_INT_CONST_H_

#define INT8_C(c)   c
#define INT16_C(c)  c
#define INT32_C(c)  c
#define INT64_C(c)  c ## LL

#define UINT8_C(c)  c
#define UINT16_C(c) c
#define UINT32_C(c) c ## U
#define UINT64_C(c) c ## ULL

#define INTMAX_C(c)     c ## LL
#define UINTMAX_C(c)    c ## ULL

#endif /* _CPUTWO_INT_CONST_H_ */
