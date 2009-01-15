//****************************************************************************
//**
//**    exception.cpp
//**		system exception handlers. These are registered during system
//**		initialization and called automatically when they are encountered
//**
//****************************************************************************

#include "exception.h"
#include <hal.h>
#include "panic.h"

#define intstart() \
	asm("cli");	\
	asm("sub $4, %ebp");
		
//! divide by 0 fault
void  divide_by_zero_fault (unsigned int eip, unsigned int cs, unsigned int flags) {

	intstart ();
	kernel_panic ("Divide by 0 at physical address [0x%x:0x%x] flags [0x%x]", cs, eip, flags);
	for (;;);
}

//! single step
void  single_step_trap (unsigned int eip, unsigned int cs, unsigned int flags) {

	intstart ();
	kernel_panic ("Single step at physical address [0x%x:0x%x] flags [0x%x]",cs,eip, flags);
	for (;;);
}

//! non maskable  trap
void  nmi_trap (unsigned int eip, unsigned int cs, unsigned int flags) {

	intstart ();
	kernel_panic ("NMI trap at physical address [0x%x:0x%x] flags [0x%x]",cs,eip, flags);
	for (;;);
}

//! breakpoint hit
void  breakpoint_trap (unsigned int eip, unsigned int cs, unsigned int flags) {

	intstart ();
	kernel_panic ("Breakpoint trap at physical address [0x%x:0x%x] flags [0x%x]",cs,eip, flags);
	for (;;);
}

//! overflow
void  overflow_trap (unsigned int eip, unsigned int cs, unsigned int flags) {

	intstart ();
	kernel_panic ("Overflow trap at physical address [0x%x:0x%x] flags [0x%x]",cs,eip, flags);
	for (;;);
}

//! bounds check
void  bounds_check_fault (unsigned int eip, unsigned int cs, unsigned int flags) {

	intstart ();
	kernel_panic ("Bounds check fault at physical address [0x%x:0x%x] flags [0x%x]",cs,eip, flags);
	for (;;);
}

//! invalid opcode / instruction
void  invalid_opcode_fault (unsigned int eip, unsigned int cs, unsigned int flags) {

	intstart ();
	kernel_panic ("Invalid opcode at physical address [0x%x:0x%x] flags [0x%x]",cs,eip, flags);
	for (;;);
}

//! device not available
void  no_device_fault (unsigned int eip, unsigned int cs, unsigned int flags) {

	intstart ();
	kernel_panic ("Device not found fault at physical address [0x%x:0x%x] flags [0x%x]",cs,eip, flags);
	for (;;);
}

//! double fault
void  double_fault_abort (unsigned int err, unsigned int eip, unsigned int cs, unsigned int flags) {

	intstart ();
	kernel_panic ("Double fault at physical address [0x%x:0x%x] flags [0x%x]",cs,eip, flags);
	for (;;);
}

//! invalid Task State Segment (TSS)
void  invalid_tss_fault (unsigned int err, unsigned int eip, unsigned int cs, unsigned int flags) {

	intstart ();
	kernel_panic ("Invalid TSS at physical address [0x%x:0x%x] flags [0x%x]",cs,eip, flags);
	for (;;);
}

//! segment not present
void  no_segment_fault (unsigned int err, unsigned int eip, unsigned int cs, unsigned int flags) {

	intstart ();
	kernel_panic ("Invalid segment at physical address [0x%x:0x%x] flags [0x%x]",cs,eip, flags);
	for (;;);
}

//! stack fault
void  stack_fault (unsigned int err, unsigned int eip, unsigned int cs, unsigned int flags) {

	intstart ();
	kernel_panic ("Stack fault at physical address [0x%x:0x%x] flags [0x%x]",cs,eip, flags);
	for (;;);
}

//! general protection fault
void  general_protection_fault (unsigned int err, unsigned int eip, unsigned int cs, unsigned int flags) {

	intstart ();
	kernel_panic ("General Protection Fault at physical address [0x%x:0x%x] flags [0x%x]", cs, eip, flags);
	for (;;);
}

//! page fault
//void  page_fault (unsigned int flags, unsigned int err, unsigned int cs, unsigned int eip) {
void  page_fault (unsigned int err, unsigned int eip, unsigned int cs, unsigned int flags) {

	intstart ();
	
	int faultAddr = 0;
	asm volatile("mov %%cr2, %0" : "=r" (faultAddr));

	// The error code gives us details of what happened.
    int present   = !(err & 0x1); // Page not present
    int rw = err & 0x2;           // Write operation?
    int us = err & 0x4;           // Processor was in user-mode?
    int reserved = err & 0x8;     // Overwritten CPU-reserved bits of page entry?
    int id = err & 0x10;          // Caused by an instruction fetch?

	kernel_panic ("Page Fault at 0x%x:0x%x refrenced memory at 0x%x\npr: %x\nrw: %x\nus: %x\nrs: %x\nid: %x", cs, eip, faultAddr, present, rw, us, reserved, id);
	
	for (;;);
}

//! Floating Point Unit (FPU) error
void  fpu_fault (unsigned int eip, unsigned int cs, unsigned int flags) {

	intstart ();
	kernel_panic ("FPU Fault at physical address [0x%x:0x%x] flags [0x%x]",cs,eip, flags);
	for (;;);
}

//! alignment check
void  alignment_check_fault (unsigned int err, unsigned int eip, unsigned int cs, unsigned int flags) {

	intstart ();
	kernel_panic ("Alignment Check at physical address [0x%x:0x%x] flags [0x%x]",cs,eip, flags);
	for (;;);
}

//! machine check
void  machine_check_abort (unsigned int eip, unsigned int cs, unsigned int flags) {

	intstart ();
	kernel_panic ("Machine Check at physical address [0x%x:0x%x] flags [0x%x]",cs,eip, flags);
	for (;;);
}

//! Floating Point Unit (FPU) Single Instruction Multiple Data (SIMD) error
void  simd_fpu_fault (unsigned int eip, unsigned int cs, unsigned int flags) {

	intstart ();
	kernel_panic ("FPU SIMD fault at physical address [0x%x:0x%x] flags [0x%x]",cs,eip, flags);
	for (;;);
}
