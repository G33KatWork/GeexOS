#ifndef _SORTEDARRAY_H
#define _SORTEDARRAY_H
//****************************************************************************
//**
//**    sortedArray.h
//**    - This array is insertion sorted - it always remains in a sorted state (between calls).
//**      It can store anything that can be cast to a void* -- so a unsigned, or any pointer.
//**
//****************************************************************************
//============================================================================
//    INTERFACE REQUIRED HEADERS
//============================================================================

#include <stdint.h>

//============================================================================
//    INTERFACE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================

typedef void* type_t;
typedef char (*lessthan_predicate_t)(type_t,type_t);

//============================================================================
//    INTERFACE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    INTERFACE STRUCTURES / UTILITY CLASSES
//============================================================================

typedef struct
{
    type_t *array;
    unsigned size;
    unsigned max_size;
    lessthan_predicate_t less_than;
} sorted_array_t;

//============================================================================
//    INTERFACE DATA DECLARATIONS
//============================================================================
//============================================================================
//    INTERFACE FUNCTION PROTOTYPES
//============================================================================

// Create an sorted array.
sorted_array_t create_sorted_array(unsigned max_size, lessthan_predicate_t less_than);
sorted_array_t place_sorted_array(void *addr, unsigned max_size, lessthan_predicate_t less_than);

// Destroy an sorted array.
void destroy_sorted_array(sorted_array_t *array);

// Add an item into the array.
void insert_sorted_array(type_t item, sorted_array_t *array);

// Lookup the item at index i.
type_t lookup_sorted_array(unsigned i, sorted_array_t *array);

// Deletes the item at location i from the array.
void remove_sorted_array(unsigned i, sorted_array_t *array);
//============================================================================
//    INTERFACE OBJECT CLASS DEFINITIONS
//============================================================================
//============================================================================
//    INTERFACE TRAILING HEADERS
//============================================================================
//****************************************************************************
//**
//**    END kmalloc.h
//**
//****************************************************************************
#endif
