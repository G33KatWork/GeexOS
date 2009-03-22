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

uint32_t kmalloc_a(uint32_t size)
{
	if (placement_address & 0xFFFFF000)
    {
        // Align the placement address;
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }

	uint32_t tmp = placement_address;
	placement_address += size;
	return tmp;
}
