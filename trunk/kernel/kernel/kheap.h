#ifndef _KHEAP_H
#define _KHEAP_H
//****************************************************************************
//**
//**    kheap.h
//**    - Heap management
//**
//****************************************************************************
//============================================================================
//    INTERFACE REQUIRED HEADERS
//============================================================================

#include <stdint.h>
#include "sortedArray.h"

//============================================================================
//    INTERFACE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================

#define KHEAP_START         0xC0000000
#define KHEAP_INITIAL_SIZE  0x100000

#define HEAP_INDEX_SIZE   0x20000
#define HEAP_MAGIC        0x123890AB
#define HEAP_MIN_SIZE     0x70000

//============================================================================
//    INTERFACE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    INTERFACE STRUCTURES / UTILITY CLASSES
//============================================================================

/**
   Size information for a hole/block
**/
typedef struct
{
    unsigned magic;   // Magic number, used for error checking and identification.
    char is_hole;   // 1 if this is a hole. 0 if this is a block.
    unsigned size;    // size of the block, including the end footer.
} header_t;

typedef struct
{
    unsigned magic;     // Magic number, same as in header_t.
    header_t *header; // Pointer to the block header.
} footer_t;

typedef struct
{
    sorted_array_t index;
    unsigned start_address; // The start of our allocated space.
    unsigned end_address;   // The end of our allocated space. May be expanded up to max_address.
    unsigned max_address;   // The maximum address the heap can be expanded to.
    char supervisor;     // Should extra pages requested by us be mapped as supervisor-only?
    char readonly;       // Should extra pages requested by us be mapped as read-only?
} heap_t;

//============================================================================
//    INTERFACE DATA DECLARATIONS
//============================================================================
//============================================================================
//    INTERFACE FUNCTION PROTOTYPES
//============================================================================

// Create a new heap.
heap_t *create_heap(unsigned start, unsigned end, unsigned max, char supervisor, char readonly);

// Releases a block allocated with 'alloc'.
//void free(void *p, heap_t *heap);


//page aligned
unsigned kmalloc_a(unsigned size);

//returns physical address
unsigned kmalloc_p(unsigned size, unsigned *phys);

//page aligned and returns physical address
unsigned kmalloc_ap(unsigned size, unsigned *phys);

//normal
unsigned kmalloc(unsigned size);
//release allocated memory
void kfree(void *p);

//============================================================================
//    INTERFACE OBJECT CLASS DEFINITIONS
//============================================================================
//============================================================================
//    INTERFACE TRAILING HEADERS
//============================================================================
//****************************************************************************
//**
//**    END kheap.h
//**
//****************************************************************************
#endif
