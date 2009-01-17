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


#include <stdint.h>


#define cpuid(func,ax,bx,cx,dx)\
	__asm__ __volatile__ ("cpuid":\
	"=a" (ax), "=b" (bx), "=c" (cx), "=d" (dx) : "a" (func));
	

// initialize the processors
int i86_cpu_initialize ();

// shutdown the processors
void i86_cpu_shutdown ();

// get cpu vender
char* i86_cpu_get_vendor ();

//****************************************************************************
//**
//**    END [FILE NAME]
//**
//****************************************************************************
#endif
