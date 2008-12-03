
#ifndef _EXCEPTION_H
#define _EXCEPTION_H
//****************************************************************************
//**
//**    exception.h
//**		system exception handlers. These are registered during system
//**		initialization and called automatically when they are encountered
//**
//****************************************************************************

//============================================================================
//    INTERFACE REQUIRED HEADERS
//============================================================================
//============================================================================
//    INTERFACE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================
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

//! exception handlers

//! divide by 0
extern void divide_by_zero_fault (unsigned int cs, 
                      unsigned int eip, unsigned int eflags);

//! single step
extern void single_step_trap (unsigned int cs, 
                      unsigned int eip, unsigned int eflags);

//! non maskable interrupt trap
extern void nmi_trap (unsigned int cs, 
                      unsigned int eip, unsigned int eflags);

//! breakpoint hit
extern void breakpoint_trap (unsigned int cs, 
                      unsigned int eip, unsigned int eflags);

//! overflow
extern void overflow_trap (unsigned int cs, 
                      unsigned int eip, unsigned int eflags);

//! bounds check
extern void bounds_check_fault (unsigned int cs, 
                      unsigned int eip, unsigned int eflags);

//! invalid opcode / instruction
extern void invalid_opcode_fault (unsigned int cs, 
                      unsigned int eip, unsigned int eflags);

//! device not available
extern void no_device_fault (unsigned int cs, 
                      unsigned int eip, unsigned int eflags);

//! double fault
extern void double_fault_abort (unsigned int cs, unsigned int err,
                      unsigned int eip, unsigned int eflags);

//! invalid Task State Segment (TSS)
extern void invalid_tss_fault (unsigned int cs, unsigned int err,
                      unsigned int eip, unsigned int eflags);

//! segment not present
extern void no_segment_fault (unsigned int cs, unsigned int err,
                      unsigned int eip, unsigned int eflags);

//! stack fault
extern void stack_fault (unsigned int cs, unsigned int err, 
                      unsigned int eip, unsigned int eflags);

//! general protection fault
extern void general_protection_fault (unsigned int cs, unsigned int err, 
                      unsigned int eip, unsigned int eflags);

//! page fault
extern void page_fault (unsigned int cs, unsigned int err, 
                      unsigned int eip, unsigned int eflags);

//! Floating Point Unit (FPU) error
extern void fpu_fault (unsigned int cs, 
                      unsigned int eip, unsigned int eflags);

//! alignment check
extern void alignment_check_fault (unsigned int cs, unsigned int err,
                      unsigned int eip, unsigned int eflags);

//! machine check
extern void machine_check_abort (unsigned int cs, 
                      unsigned int eip, unsigned int eflags);

//! Floating Point Unit (FPU) Single Instruction Multiple Data (SIMD) error
extern void simd_fpu_fault (unsigned int cs, 
                      unsigned int eip, unsigned int eflags);

//============================================================================
//    INTERFACE OBJECT CLASS DEFINITIONS
//============================================================================
//============================================================================
//    INTERFACE TRAILING HEADERS
//============================================================================
//****************************************************************************
//**
//**    END [exception.h]
//**
//****************************************************************************

#endif
