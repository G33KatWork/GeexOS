#ifndef _KMALLOC_H
#define _KMALLOC_H

#include <lib/types.h>

void *kmalloc(size_t s);
void *calloc(unsigned int n, size_t s);

void free(void* b);

#endif
