#include <arch/i386/gdt.h>
#include <lib.h>

#define I86_GDT_ENTRY_COUNT     5

struct gdt_entry gdt[I86_GDT_ENTRY_COUNT];
struct gdt_ptr gp;

void gdt_set_descriptor(uint16_t i, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);
void gdt_install(void);
extern void gdt_flush(void);

void gdt_init()
{
    //FIXME: use defined constants
    gdt_set_descriptor (0, 0, 0, 0, 0);
    gdt_set_descriptor (1, 0, 0xffffffff, 0x9A, 0xCF);
	gdt_set_descriptor (2, 0, 0xffffffff, 0x92, 0xCF);
	
	//Realmode code/data
	gdt_set_descriptor (3, 0, 0xfffff, 0x9E, 0x00);
	gdt_set_descriptor (4, 0, 0xfffff, 0x92, 0x00);
	
    gdt_install();
}

void gdt_set_descriptor(uint16_t i, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
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
	gdt[i].granularity	|= (granularity & 0xf0);
}

void gdt_install()
{
    //update pointer
    gp.size = (sizeof(struct gdt_entry) * I86_GDT_ENTRY_COUNT) - 1;
	gp.offset = (uint32_t)&gdt;
	
	//load gdt
    gdt_flush();
}
