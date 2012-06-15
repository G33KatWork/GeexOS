#ifndef _TYPES_H_
#define _TYPES_H_

#define NULL ((void*)0)

#ifdef UNUSED 
#elif defined(__GNUC__) 
# define UNUSED(x) UNUSED_ ## x __attribute__((unused)) 
#elif defined(__LCLINT__) 
# define UNUSED(x) /*@unused@*/ x 
#else 
# define UNUSED(x) x 
#endif

#if defined(__GNUC__)
#define offsetof(type, member)	__builtin_offsetof(type, member)
#else
#define offsetof(st, m) \
     ((size_t) ( (char *)&((st *)0)->m - (char *)0 ))
#endif

#ifdef ARCH_I386

#include <arch/i386/types.h>

#else
#error "Unknown architecture"
#endif


#endif
