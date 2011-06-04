#ifndef _LIB_H
#define _LIB_H

#include <types.h>

void memset(void *dest, int val, size_t count);
void *memcpy(void *dest, const void* src, size_t count);
size_t strlen(const char *str);
int strcmp(const char *s1, const char *s2);

void panic(const char* message);

#endif
