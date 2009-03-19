#include "gdt.h"
#include <string.h>

void gdt_set_descriptor(uint32_t i, uint64_t base, uint64_t limit, uint8_t access, uint8_t granularity)
{
	if (i > I86_GDT_ENTRY_COUNT)
		return;

	// null out the descriptor
	memset ((void*)&gdt[i], 0, sizeof (struct gdt_entry));

	// set limit and base addresses
	gdt[i].base_low		= (uint16_t)(base & 0xffff);
	gdt[i].base_middle	= (uint8_t)((base >> 16) & 0xff);
	gdt[i].base_high	= (uint8_t)((base >> 24) & 0xff);
	gdt[i].limit_low	= (uint16_t)(limit & 0xffff);

	// set flags and grandularity bytes
	gdt[i].access		 = access;
	gdt[i].granularity	 = (uint8_t)((limit >> 16) & 0x0f);
	gdt[i].granularity	|= granularity & 0xf0;
}

void gdt_install()
{
	//FIXME: I86_GDT_ENTRY_COUNT * 2????
	gp.size = (sizeof(struct gdt_entry) * I86_GDT_ENTRY_COUNT) - 1;
	gp.offset = (unsigned int)&gdt;
	
	gdt_set_descriptor(0, 0, 0, 0, 0);
	gdt_set_descriptor (1,0,0xffffffff,
		I86_GDT_DESC_READWRITE|I86_GDT_DESC_EXEC_CODE|I86_GDT_DESC_CODEDATA|I86_GDT_DESC_MEMORY,
		I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);
	gdt_set_descriptor (2,0,0xffffffff,
		I86_GDT_DESC_READWRITE|I86_GDT_DESC_CODEDATA|I86_GDT_DESC_MEMORY,
		I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);
	
	gdt_flush();
}