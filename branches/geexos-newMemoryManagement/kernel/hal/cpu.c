//****************************************************************************
//**
//**    cpu.c
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
#include "cpu.h"
#include "gdt.h"
#include "idt.h"

// ************************************ Public ***********************************

// Initializes cpu resources
int i86_cpu_initialize () {

	//! initialize processor tables
	i86_gdt_initialize ();
	i86_idt_initialize (0x8);

	return 0;
}

// shuts down cpu resources...Nothing to do yet
void i86_cpu_shutdown () {

}

// returns vendor name of cpu
char* i86_cpu_get_vendor () {

	static char	vendor[13] = {0};
	int a;
	
	int v1;
	int v2;
	int v3;

	cpuid(0x0, a, v1, v2, v3);
	
	vendor[3] = (char)(v1 >> 24);
	vendor[2] = (char)(v1 >> 16);
	vendor[1] = (char)(v1 >> 8);
	vendor[0] = (char)(v1);
	
	vendor[7] = (char)(v3 >> 24);
	vendor[6] = (char)(v3 >> 16);
	vendor[5] = (char)(v3 >> 8);
	vendor[4] = (char)(v3);
	
	vendor[11] = (char)(v2 >> 24);
	vendor[10] = (char)(v2 >> 16);
	vendor[9] = (char)(v2 >> 8);
	vendor[8] = (char)v2;

	return vendor;
}

//****************************************************************************
//**
//**    END[cpu.c]
//**
//****************************************************************************
