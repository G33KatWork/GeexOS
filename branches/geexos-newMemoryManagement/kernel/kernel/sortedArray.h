#ifndef _SORTEDARRAY_H
#define _SORTEDARRAY_H
//****************************************************************************
//**
//**    sortedArray.h
//**    - This array is insertion sorted - it always remains in a sorted state (between calls).
//**      It can store anything that can be cast to a void* -- so a unsigned, or any pointer.
//**
//****************************************************************************
#include <stdint.h>


typedef void* type_t;
typedef int8_t (*lessthan_predicate_t)(type_t,type_t);


typedef struct
{
    type_t *array;
    uint32_t size;
    uint32_t max_size;
    lessthan_predicate_t less_than;
} sorted_array_t;

// Create a sorted array.
sorted_array_t create_sorted_array(uint32_t max_size, lessthan_predicate_t less_than);
sorted_array_t place_sorted_array(void *addr, uint32_t max_size, lessthan_predicate_t less_than);

// Destroy a sorted array.
void destroy_sorted_array(sorted_array_t *array);

// Add an item into the array.
void insert_sorted_array(type_t item, sorted_array_t *array);

// Lookup the item at index i.
type_t lookup_sorted_array(uint32_t i, sorted_array_t *array);

// Deletes the item at location i from the array.
void remove_sorted_array(uint32_t i, sorted_array_t *array);

//****************************************************************************
//**
//**    END kmalloc.h
//**
//****************************************************************************
#endif
