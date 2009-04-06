#ifndef _ISR_H
#define _ISR_H

#include <stdint.h>


// The following devices use PIC 1 to generate interrupts
#define		IRQ_TIMER			32
#define		IRQ_KEYBOARD		33
#define		IRQ_SERIAL2			34
#define		IRQ_SERIAL1			35
#define		IRQ_PARALLEL2		36
#define		IRQ_DISKETTE		37
#define		IRQ_PARALLEL1		38

// The following devices use PIC 2 to generate interrupts
#define		IRQ_CMOSTIMER		39
#define		IRQ_CGARETRACE		40
#define		IRQ_AUXILIARY		41
#define		IRQ_FPU				42
#define		IRQ_HDC				43


typedef struct registers
{
	uint32_t ds;										// Data segment selector
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;	// Pushed by pusha.
	uint32_t int_no, err_code;							// Interrupt number and error code (if applicable)
	uint32_t eip, cs, eflags, useresp, ss;				// Pushed by the processor automatically.
} registers_t;

void isr_handler(registers_t regs);
void irq_handler(registers_t regs);

typedef void (*isr_t)(registers_t);
void register_interrupt_handler(uint8_t n, isr_t handler);

#endif