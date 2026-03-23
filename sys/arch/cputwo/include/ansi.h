#ifndef _ANSI_H_
#define _ANSI_H_

#include <sys/cdefs.h>
#include <machine/int_types.h>

#define _BSD_CLOCK_T_       unsigned int
#if defined(__minix)
#define _BSD_PTRDIFF_T_     int
#define _BSD_SIZE_T_        unsigned int
#define _BSD_SSIZE_T_       int
#else
#ifdef __PTRDIFF_TYPE__
#define _BSD_PTRDIFF_T_     __PTRDIFF_TYPE__
#define _BSD_SSIZE_T_       __PTRDIFF_TYPE__
#else
#define _BSD_PTRDIFF_T_     long int
#define _BSD_SSIZE_T_       long int
#endif
#ifdef __SIZE_TYPE__
#define _BSD_SIZE_T_        __SIZE_TYPE__
#else
#define _BSD_SIZE_T_        unsigned long int
#endif
#endif /* defined(__minix) */
#define _BSD_TIME_T_        __int64_t
#define _BSD_CLOCKID_T_     int
#define _BSD_TIMER_T_       int
#define _BSD_SUSECONDS_T_   int
#define _BSD_USECONDS_T_    unsigned int
#ifdef __WCHAR_TYPE__
#define _BSD_WCHAR_T_       __WCHAR_TYPE__
#else
#define _BSD_WCHAR_T_       int
#endif
#ifdef __WINT_TYPE__
#define _BSD_WINT_T_        __WINT_TYPE__
#else
#define _BSD_WINT_T_        int
#endif

#endif /* _ANSI_H_ */
