
#ifndef _EXCEPTION_H
#define _EXCEPTION_H
//****************************************************************************
//**
//**    exception.h
//**		system exception handlers. These are registered during system
//**		initialization and called automatically when they are encountered
//**
//****************************************************************************

// exception handlers

// divide by 0
void divide_by_zero_fault (unsigned int eip, unsigned int cs, unsigned int flags);

// single step
void single_step_trap (unsigned int eip, unsigned int cs, unsigned int flags);

// non maskable interrupt trap
void nmi_trap (unsigned int eip, unsigned int cs, unsigned int flags);

// breakpoint hit
void breakpoint_trap (unsigned int eip, unsigned int cs, unsigned int flags);

// overflow
void overflow_trap (unsigned int eip, unsigned int cs, unsigned int flags);

// bounds check
void bounds_check_fault (unsigned int eip, unsigned int cs, unsigned int flags);

// invalid opcode / instruction
void invalid_opcode_fault (unsigned int eip, unsigned int cs, unsigned int flags);

// device not available
void no_device_fault (unsigned int eip, unsigned int cs, unsigned int flags);

// double fault
void double_fault_abort (unsigned int err, unsigned int eip, unsigned int cs, unsigned int flags);

// invalid Task State Segment (TSS)
void invalid_tss_fault (unsigned int err, unsigned int eip, unsigned int cs, unsigned int flags);

// segment not present
void no_segment_fault (unsigned int err, unsigned int eip, unsigned int cs, unsigned int flags);

// stack fault
void stack_fault (unsigned int err, unsigned int eip, unsigned int cs, unsigned int flags);

// general protection fault
void general_protection_fault (unsigned int err, unsigned int eip, unsigned int cs, unsigned int flags);

// page fault
void page_fault (unsigned int err, unsigned int eip, unsigned int cs, unsigned int flags);

// Floating Point Unit (FPU) error
void fpu_fault (unsigned int eip, unsigned int cs, unsigned int flags);

// alignment check
void alignment_check_fault (unsigned int err, unsigned int eip, unsigned int cs, unsigned int flags);

// machine check
void machine_check_abort (unsigned int eip, unsigned int cs, unsigned int flags);

// Floating Point Unit (FPU) Single Instruction Multiple Data (SIMD) error
void simd_fpu_fault (unsigned int eip, unsigned int cs, unsigned int flags);


//****************************************************************************
//**
//**    END [exception.h]
//**
//****************************************************************************

#endif
