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
char* strncpy(char *s1, const char *s2, size_t n);
int strcmp(const char *s1, const char *s2);
int stricmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
char* strchr(const char *s, int c);
char* strrchr(const char *s, int c);
char* strcat(char *dst, const char *src);
char* strncat(char *dst, const char *src, size_t n);
void* memcpy(void *dest, const void* src, size_t count);
void* memmove(void *dest, const void* src, size_t count);
int memcmp(const void *s1, const void *s2, size_t len);

#endif
