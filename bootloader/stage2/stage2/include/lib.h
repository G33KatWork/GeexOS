#ifndef _LIB_H
#define _LIB_H

#include <types.h>

//gcc builtin va_list
#define va_start(v,l) __builtin_va_start(v,l)
#define va_arg(v,l)   __builtin_va_arg(v,l)
#define va_end(v)     __builtin_va_end(v)
#define va_copy(d,s)  __builtin_va_copy(d,s)
typedef __builtin_va_list va_list;


void memset(void *dest, int val, size_t count);
size_t strlen(const char *str);

char* strcpy(char *dest, const char *src);
void* memcpy(void *dest, const void* src, size_t count);

#endif
