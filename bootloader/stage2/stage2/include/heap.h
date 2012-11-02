#ifndef _GXLDR_HEAP_H_
#define _GXLDR_HEAP_H_

#include <stddef.h>
#include <stdint.h>

//#define HEAPBLOCK_MAGIC	0xDEADBEEF

struct __freelist {
	size_t sz;
	struct __freelist* nx;
};

typedef struct {
	size_t MaxSize;
	struct __freelist* Freelist;
	uint32_t allocationCount;
	uint32_t deallocationCount;
} Heap;

Heap* heap_create(size_t MaxSize);
void heap_destroy(Heap* heap);

void* heap_alloc(Heap* heap, size_t len);
void heap_free(Heap* heap, void* addr);

void heap_printFreelist(Heap* heap);

#endif
