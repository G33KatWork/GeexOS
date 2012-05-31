#ifndef _LIB_H
#define _LIB_H

#include <types.h>

#define DEFAULT_HEAP_SIZE	1*1024*1024

//gcc builtin va_list
#define va_start(v,l) __builtin_va_start(v,l)
#define va_arg(v,l)   __builtin_va_arg(v,l)
#define va_end(v)     __builtin_va_end(v)
#define va_copy(d,s)  __builtin_va_copy(d,s)
typedef __builtin_va_list va_list;

void default_heap_init(void);

void* malloc(size_t len);
void free(void* p);

void memset(void *dest, int val, size_t count);
size_t strlen(const char *str);

char tolower(char c);

char* strcpy(char *dest, const char *src);
int strcmp(const char *s1, const char *s2);
int stricmp(const char *s1, const char *s2);
char* strchr(const char *s, int c);
void* memcpy(void *dest, const void* src, size_t count);
void* memmove(void *dest, const void* src, size_t count);
int memcmp(const void *s1, const void *s2, size_t len);

#endif
