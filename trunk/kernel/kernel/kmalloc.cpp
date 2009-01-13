//****************************************************************************
//**
//**    kmalloc.cpp
//**    - Simple placement memory management for kernel objects.
//**
//****************************************************************************
//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

#include "kmalloc.h"

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

// end is defined in the linker script.
extern unsigned end;
unsigned placement_address = (unsigned)&end;

//============================================================================
//    INTERFACE DATA
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTION PROTOTYPES
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTIONS
//============================================================================


//internal version of kmalloc
unsigned kmalloc_int(unsigned size, int align, unsigned *phys)
{
	// This will eventually call malloc() on the kernel heap.
    // For now, though, we just assign memory at placement_address
    // and increment it by sz. Even when we've coded our kernel
    // heap, this will be useful for use before the heap is initialised.
    if (align == 1 && (placement_address & 0xFFFFF000) )
    {
        // Align the placement address;
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    if (phys)
    {
        *phys = placement_address;
    }
    unsigned tmp = placement_address;
    placement_address += size;
    return tmp;
}

//============================================================================
//    INTERFACE FUNCTIONS
//============================================================================

//page aligned
unsigned kmalloc_a(unsigned size)
{
	return kmalloc_int(size, 1, 0);
}

//returns physical address
unsigned kmalloc_p(unsigned size, unsigned *phys)
{
	return kmalloc_int(size, 0, phys);
}

//page aligned and returns physical address
unsigned kmalloc_ap(unsigned size, unsigned *phys)
{
	return kmalloc_int(size, 1, phys);
}

//normal
unsigned kmalloc(unsigned size)
{
	return kmalloc_int(size, 0, 0);
}

//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[kmalloc.cpp]
//**
//****************************************************************************
