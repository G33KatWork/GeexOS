#ifndef _MEMORYMANAGER_H
#define _MEMORYMANAGER_H

#include <stdint.h>

/**
 * Define the beginning of the kernel heap here
**/
#define	KERNELHEAP_START	0xC0400000

uint32_t kmalloc(uint32_t size);
uint32_t kmalloc_a(uint32_t size);

#endif