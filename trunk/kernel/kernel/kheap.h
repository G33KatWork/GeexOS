#ifndef _KHEAP_H
#define _KHEAP_H
//****************************************************************************
//**
//**    kheap.h
//**    - Heap management
//**
//****************************************************************************
#include <stdint.h>
#include "sortedArray.h"


#define KHEAP_START         0xC0000000
#define KHEAP_INITIAL_SIZE  0x100000

#define HEAP_INDEX_SIZE   0x20000
#define HEAP_MAGIC        0x123890AB
#define HEAP_MIN_SIZE     0x70000

/**
   Size information for a hole/block
**/
typedef struct
{
    uint32_t magic;   // Magic number, used for error checking and identification.
    uint8_t is_hole;  // 1 if this is a hole. 0 if this is a block.
    uint32_t size;    // size of the block, including the end footer.
} header_t;

typedef struct
{
    uint32_t magic;     // Magic number, same as in header_t.
    header_t *header; // Pointer to the block header.
} footer_t;

typedef struct
{
    sorted_array_t index;
    uint32_t start_address; // The start of our allocated space.
    uint32_t end_address;   // The end of our allocated space. May be expanded up to max_address.
    uint32_t max_address;   // The maximum address the heap can be expanded to.
    uint8_t supervisor;     // Should extra pages requested by us be mapped as supervisor-only?
    uint8_t readonly;       // Should extra pages requested by us be mapped as read-only?
} heap_t;

extern uint32_t placement_address;
extern heap_t *kheap;


// ************************************ Public ***********************************

// Create a new heap.
heap_t *create_heap(uint32_t start, uint32_t end, uint32_t max, uint8_t supervisor, uint8_t readonly);

//page aligned
uint32_t kmalloc_a(uint32_t size);

//returns physical address
uint32_t kmalloc_p(uint32_t size, uint32_t *phys);

//page aligned and returns physical address
uint32_t kmalloc_ap(uint32_t size, uint32_t *phys);

//normal
uint32_t kmalloc(uint32_t size);

//release allocated memory
void kfree(void *p);

//****************************************************************************
//**
//**    END kheap.h
//**
//****************************************************************************
#endif
