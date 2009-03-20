#include "isr.h"
#include "../../DebugDisplay.h"

void isr_handler(registers_t regs);

void isr_handler(registers_t regs)
{
	DebugPrintf("Interrupt: %u\n", regs.int_no);
}
