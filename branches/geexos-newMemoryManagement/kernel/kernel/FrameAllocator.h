#ifndef _PAGEALLOCATOR_H
#define _PAGEALLOCATOR_H

#include <stdint.h>

/**
 * Initialize allocator for pages
**/
void init_allocator(uint32_t memorySize);

/**
 * Search for an unused frame and return its physical address
**/
uint32_t* allocate_frame(void);

/**
 * Take the physical address of a frame and mark it as free
**/
void free_frame(uint32_t* physAddr);

#endif