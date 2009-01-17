//****************************************************************************
//**
//**    sortedArray.cpp
//**    - This array is insertion sorted - it always remains in a sorted state (between calls).
//**      It can store anything that can be cast to a void* -- so a unsigned, or any pointer.
//**
//****************************************************************************
//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

#include "sortedArray.h"
#include "kheap.h"
#include <string.h>

//============================================================================
//    IMPLEMENTATION PRIVATE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE STRUCTURES / UTILITY CLASSES
//============================================================================
//============================================================================
//    IMPLEMENTATION REQUIRED EXTERNAL REFERENCES (AVOID)
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE DATA
//============================================================================
//============================================================================
//    INTERFACE DATA
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTION PROTOTYPES
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTIONS
//============================================================================
//============================================================================
//    INTERFACE FUNCTIONS
//============================================================================

char standard_lessthan_predicate(type_t a, type_t b)
{
    return (a<b)?1:0;
}

sorted_array_t create_sorted_array(unsigned max_size, lessthan_predicate_t less_than)
{
    sorted_array_t to_ret;
    to_ret.array = (void**)kmalloc(max_size*sizeof(type_t));
    memset(to_ret.array, 0, max_size*sizeof(type_t));
    to_ret.size = 0;
    to_ret.max_size = max_size;
    to_ret.less_than = less_than;
    return to_ret;
}

sorted_array_t place_sorted_array(void *addr, unsigned max_size, lessthan_predicate_t less_than)
{
    sorted_array_t to_ret;
    to_ret.array = (type_t*)addr;
    memset(to_ret.array, 0, max_size*sizeof(type_t));
    to_ret.size = 0;
    to_ret.max_size = max_size;
    to_ret.less_than = less_than;
    return to_ret;
}

void destroy_sorted_array(sorted_array_t *array)
{
//    kfree(array->array);
}

void insert_sorted_array(type_t item, sorted_array_t *array)
{
    //TODO: write ASSERT macro
    //ASSERT(array->less_than);
    unsigned iterator = 0;
    while (iterator < array->size && array->less_than(array->array[iterator], item))
        iterator++;
    if (iterator == array->size) // just add at the end of the array.
        array->array[array->size++] = item;
    else
    {
        type_t tmp = array->array[iterator];
        array->array[iterator] = item;
        while (iterator < array->size)
        {
            iterator++;
            type_t tmp2 = array->array[iterator];
            array->array[iterator] = tmp;
            tmp = tmp2;
        }
        array->size++;
    }
}

type_t lookup_sorted_array(unsigned i, sorted_array_t *array)
{
	//TODO: write ASSERT macro
    //ASSERT(i < array->size);
    return array->array[i];
}

void remove_sorted_array(unsigned i, sorted_array_t *array)
{
    while (i < array->size)
    {
        array->array[i] = array->array[i+1];
        i++;
    }
    array->size--;
}

//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[sortedArray.cpp]
//**
//****************************************************************************
