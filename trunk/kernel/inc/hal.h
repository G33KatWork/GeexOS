#ifndef _HAL_H
#define _HAL_H
//****************************************************************************
//**
//**    hal.h
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

#include <stdint.h>

#define far
#define near


// ************************************ Public ***********************************

// initialize hardware abstraction layer
int				hal_initialize ();

// shutdown hardware abstraction layer
int				hal_shutdown ();

// generates interrupt
void			geninterrupt (int n);

// notifies hal interrupt is done
void			interruptdone (unsigned int intno);

// output sound to speaker
void			sound (unsigned frequency);

// read byte from device using port mapped io
unsigned char	inportb (unsigned short portid);

// write byte to device through port mapped io
void				outportb (unsigned short portid, unsigned char value);

// enables all hardware interrupts
void				enable ();

//! disable all hardware interrupts
void				disable ();

// sets new interrupt vector
void				setvect (int intno, void (far *vect) ( ) );

// returns current interrupt vector
void				(far *	getvect (int intno)) ( );

// returns cpu vender
const char*		get_cpu_vendor ();

// returns current tick count (only for demo)
int				get_tick_count ();


//****************************************************************************
//**
//**    END [hal.h]
//**
//****************************************************************************
#endif
