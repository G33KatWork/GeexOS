#ifndef _HAL_H
#define _HAL_H
//****************************************************************************
//**
//**    Hal.h
//**		Hardware Abstraction Layer Interface
//**
//**	The Hardware Abstraction Layer (HAL) provides an abstract interface
//**	to control the basic motherboard hardware devices. This is accomplished
//**	by abstracting hardware dependencies behind this interface.
//**
//**	All routines and types are declared extern and must be defined within
//**	external libraries to define specific hal implimentations.
//**
//****************************************************************************

#ifndef ARCH_X86
#pragma error "HAL not implimented for this platform"
#endif

//============================================================================
//    INTERFACE REQUIRED HEADERS
//============================================================================

#include <stdint.h>

//============================================================================
//    INTERFACE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================

#define far
#define near

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

//! initialize hardware abstraction layer
extern	int				hal_initialize ();

//! shutdown hardware abstraction layer
extern	int				hal_shutdown ();

//! generates interrupt
extern	void			geninterrupt (int n);

//! notifies hal interrupt is done
extern  void			interruptdone (unsigned int intno);

//! output sound to speaker
extern	void			sound (unsigned frequency);

//! read byte from device using port mapped io
extern	unsigned char	inportb (unsigned short portid);

//! write byte to device through port mapped io
extern void				outportb (unsigned short portid, unsigned char value);

//! enables all hardware interrupts
extern void				enable ();

//! disable all hardware interrupts
extern void				disable ();

//! sets new interrupt vector
extern void				setvect (int intno, void (far &vect) ( ) );

//! returns current interrupt vector
extern void				(far *	getvect (int intno)) ( );

//! returns cpu vender
extern const char*		get_cpu_vendor ();

//! returns current tick count (only for demo)
extern int				get_tick_count ();

//============================================================================
//    INTERFACE OBJECT CLASS DEFINITIONS
//============================================================================
//============================================================================
//    INTERFACE TRAILING HEADERS
//============================================================================
//****************************************************************************
//**
//**    END [Hal.h]
//**
//****************************************************************************
#endif
