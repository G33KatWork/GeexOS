
//****************************************************************************
//**
//**    Hal.cpp
//**		Hardware Abstraction Layer for i86 architecture
//**
//**	The Hardware Abstraction Layer (HAL) provides an abstract interface
//**	to control the basic motherboard hardware devices. This is accomplished
//**	by abstracting hardware dependencies behind this interface.
//**
//****************************************************************************

#ifndef ARCH_X86
#error "[hal.cpp for i86] requires i86 architecture. Define ARCH_X86"
#endif

//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

#include <hal.h>
#include "cpu.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"

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

//! initialize hardware devices
int hal_initialize () {

	//! intialize i86 specific devices
	i86_cpu_initialize ();
	i86_pic_initialize (0x20, 0x28); // first PIC IRQ begin: 0x20, sencond PIC: 0x28
	i86_pit_initialize ();
	i86_pit_start_counter (100,I86_PIT_OCW_COUNTER_0, I86_PIT_OCW_MODE_SQUAREWAVEGEN);
	
	//! enable interrupts
	enable();
	
	return 0;
}

//! shutdown hardware devices
int hal_shutdown () {

	i86_cpu_shutdown ();
	return 0;
}

//! generate interrupt call
void geninterrupt (int n) {
	asm volatile (	"movb %0, %%al;"
			"movb %%al, (genint+1);"
			"jmp genint;"
		"genint:"
			"int $0;"
			:"=g"(n)
		);
}

//! notifies hal interrupt is done
void interruptdone (unsigned int intno) {
	//! insure its a valid hardware irq
	if (intno > 16)
		return;

	//! test if we need to send end-of-interrupt to second pic
	if (intno >= 8)
		i86_pic_send_command (I86_PIC_OCW2_MASK_EOI, 1);

	//! always send end-of-interrupt to primary pic
	i86_pic_send_command (I86_PIC_OCW2_MASK_EOI, 0);
}


//! output sound to speaker
void sound (unsigned frequency) {

	//! sets frequency for speaker. frequency of 0 disables speaker
	outportb (0x61, 3 | (unsigned char)(frequency<<2) );
}


//! read byte from device using port mapped io
unsigned char inportb (unsigned short portid) {
	unsigned char ret;
	  
	asm volatile("inb %1, %0" : "=a" (ret) : "dN" (portid));
	
	return ret;
}


//! write byte to device through port mapped io
void outportb (unsigned short portid, unsigned char value) {
    asm volatile ("outb %1, %0" : : "dN" (portid), "a" (value));
}


//! enable all hardware interrupts
void enable () {
	asm("sti");
}


//! disable all hardware interrupts
void disable () {
	asm("cli");
}


//! sets new interrupt vector
void setvect (int intno, void (far &vect) ( ) ) {

	//! install interrupt handler! This overwrites prev interrupt descriptor
	i86_install_ir (intno, I86_IDT_DESC_PRESENT | I86_IDT_DESC_BIT32,
		0x8, vect);
}


//! returns current interrupt vector
void (far * getvect (int intno)) ( ) {

	//! get the descriptor from the idt
	idt_descriptor* desc = i86_get_ir (intno);
	if (!desc)
		return 0;

	//! get address of interrupt handler
	uint32_t addr = desc->baseLo | (desc->baseHi << 16);

	//! return interrupt handler
	I86_IRQ_HANDLER irq = (I86_IRQ_HANDLER)addr;
	return irq;
}


//! returns cpu vender
const char* get_cpu_vendor () {

	return i86_cpu_get_vendor();
}


//! returns current tick count (only for demo)
int get_tick_count () {

	return i86_pit_get_tick_count();
}

//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[Hal.cpp]
//**
//****************************************************************************
