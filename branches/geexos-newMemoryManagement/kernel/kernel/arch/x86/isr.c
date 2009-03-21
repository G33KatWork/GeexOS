#include "isr.h"
#include "pic.h"
#include "../../DebugDisplay.h"
#include "idt.h"

isr_t interrupt_handlers[I86_IDT_MAX_ENTRY_COUNT];

void isr_handler(registers_t regs)
{
	if(interrupt_handlers[regs.int_no] != 0)
	{
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
	else
		DebugPrintf(" unhandled interrupt: %u\n", regs.int_no);
}

void irq_handler(registers_t regs)
{
	interrupt_handled(regs.int_no);
	
	if(interrupt_handlers[regs.int_no] != 0)
	{
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
}

void register_interrupt_handler(uint8_t n, isr_t handler)
{
	interrupt_handlers[n] = handler;
}
