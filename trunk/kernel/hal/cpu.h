#ifndef _CPU_H_INCLUDED
# define _CPU_H_INCLUDED
//****************************************************************************
//**
//**    cpu.h
//**
//**	This is the processor interface. Everything outside of this module
//**	must use this interface when working on processor data.
//**
//**	A processor is a module that manages the very basic data structures
//**	and data within the system. The processor interface provides the interface
//**	for managing processors, processor cores, accessing processor structures,
//**	and more
//**
//****************************************************************************

#ifndef ARCH_X86
#error "[cpu.h] platform not implimented. Define ARCH_X86 for HAL"
#endif

//============================================================================
//    INTERFACE REQUIRED HEADERS
//============================================================================

#include <stdint.h>
#include "regs.h"

//============================================================================
//    INTERFACE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================

#define cpuid(func,ax,bx,cx,dx)\
	__asm__ __volatile__ ("cpuid":\
	"=a" (ax), "=b" (bx), "=c" (cx), "=d" (dx) : "a" (func));
	
//============================================================================
//    INTERFACE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    INTERFACE STRUCTURES / UTILITY CLASSES
//============================================================================
//============================================================================
//    INTERFACE DATA DECLARATIONS
//============================================================================
//============================================================================
//    INTERFACE FUNCTION PROTOTYPES
//============================================================================

//! initialize the processors
extern int i86_cpu_initialize ();

//! shutdown the processors
extern void i86_cpu_shutdown ();

//! get cpu vender
extern char* i86_cpu_get_vendor ();

//============================================================================
//    INTERFACE OBJECT CLASS DEFINITIONS
//============================================================================
//============================================================================
//    INTERFACE TRAILING HEADERS
//============================================================================
//****************************************************************************
//**
//**    END [FILE NAME]
//**
//****************************************************************************
#endif
