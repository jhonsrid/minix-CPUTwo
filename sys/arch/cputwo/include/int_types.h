#ifndef _CPUTWO_INT_TYPES_H_
#define _CPUTWO_INT_TYPES_H_

#include <sys/cdefs.h>

typedef signed char          __int8_t;
typedef unsigned char       __uint8_t;
typedef short int           __int16_t;
typedef unsigned short int __uint16_t;
typedef int                 __int32_t;
typedef unsigned int       __uint32_t;
#ifdef __COMPILER_INT64__
typedef __COMPILER_INT64__  __int64_t;
typedef __COMPILER_UINT64__ __uint64_t;
#else
typedef long long int       __int64_t;
typedef unsigned long long int __uint64_t;
#endif

typedef int                __intptr_t;
typedef unsigned int      __uintptr_t;

#endif /* _CPUTWO_INT_TYPES_H_ */
