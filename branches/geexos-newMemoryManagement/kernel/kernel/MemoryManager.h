#ifndef _MEMORYMANAGER_H
#define _MEMORYMANAGER_H

#include <stdint.h>
#include <size_t.h>

/**
 * Define the beginning of the kernel heap here
 * IMPORTANT: MUST be page aligned!
**/
#define	KHEAP_START     0xC0400000

/**
 * Define the end of the kernel heap here
**/
#define KHEAP_END       0xFFBFFFFF
//#define KHEAP_END       0xC0401000

#define KHEAP_SIZE      KHEAP_END - KHEAP_START + 1


/**
 * Initializes the kernel heap
 * IMPORTANT: Before this can happen, the paging, gdt,
 * idt and the frame allocator have to be set up and
 * functional
**/
void init_kheap(void);

/**
 * Allocates a bunch of bytes with length of size
 * IMPORTANT: Before the heap itself gets initialize,
 * which can only happen after the paging, gdt, idt and
 * the frame allocator has been initialized the maximum
 * sum of all allocated spaces must not exceed 4MB with
 * the size of the kernel binary itself! See more in the
 * documentation about the memory layout.
**/
void* kmalloc(size_t size);

/**
 * Allocates a bunch of bytes with length of size,
 * but the address is page aligned
 * See an important note in the description of kmalloc()
**/
void* kmalloc_a(size_t size);

#endif