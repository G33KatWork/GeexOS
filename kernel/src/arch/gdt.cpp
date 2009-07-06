#include <lib/types.h>
#include <lib/string.h>
#include <arch/gdt.h>

using namespace Arch;

struct gdt_entry gdt[I86_GDT_ENTRY_COUNT];
struct gdt_ptr gp;

void Arch::gdt_set_descriptor(uint16_t i, uint64_t base, uint64_t limit, uint8_t access, uint8_t granularity)
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

void Arch::gdt_install()
{
    //update pointer
    gp.size = (sizeof(struct gdt_entry) * I86_GDT_ENTRY_COUNT) - 1;
	gp.offset = (uint32_t)&gdt;
	
	//load gdt
    gdt_flush();
}
