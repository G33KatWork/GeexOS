#include "MemoryManager.h"

// end of kernel defined in linker script
extern uint32_t end;
uint32_t placement_address = (uint32_t)&end;

uint32_t kmalloc(uint32_t size)
{
	uint32_t tmp = placement_address;
	placement_address += size;
	return tmp;
}
