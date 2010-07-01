#include <arch/gdt.h>
#include <string.h>
#include <arch/tss.h>

using namespace Arch;

struct gdt_entry gdt[I86_GDT_ENTRY_COUNT];
struct gdt_ptr gp;

struct tss_entry tss;

void Arch::gdt_setup()
{
    //FIXME: Use the defined constants here instead of numeric values
    gdt_set_descriptor (0, 0, 0, 0, 0);
    gdt_set_descriptor (1, 0, 0xffffffff, 0x9A, 0xCF);
		//I86_GDT_DESC_READWRITE|I86_GDT_DESC_EXEC_CODE|I86_GDT_DESC_CODEDATA|I86_GDT_DESC_MEMORY,
		//I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);
	gdt_set_descriptor (2, 0, 0xffffffff, 0x92, 0xCF);
		//I86_GDT_DESC_READWRITE|I86_GDT_DESC_CODEDATA|I86_GDT_DESC_MEMORY,
		//I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);
	gdt_set_descriptor (3, 0, 0xffffffff, 0xFA, 0xCF);
		//I86_GDT_DESC_READWRITE|I86_GDT_DESC_CODEDATA|I86_GDT_DESC_MEMORY,
		//I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);
	gdt_set_descriptor (4, 0, 0xffffffff, 0xF2, 0xCF);
		//I86_GDT_DESC_READWRITE|I86_GDT_DESC_CODEDATA|I86_GDT_DESC_MEMORY,
		//I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);
    gdt_set_tss (5, GDT_KERNEL_DATA, 0x0);
}

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

void Arch::gdt_set_tss(uint16_t i, uint16_t ss0, uint32_t esp0)
{
    // Firstly, let's compute the base and limit of our entry into the GDT.
    uint32_t base = (uint32_t) &tss;
    uint32_t limit = base + sizeof(struct tss_entry);

    // Now, add our TSS descriptor's address to the GDT.
    gdt_set_descriptor(i, base, limit, 0xE9, 0x00);

    // Ensure the descriptor is initially zero.
    memset(&tss, 0, sizeof(struct tss_entry));

    tss.ss0  = ss0;  // Set the kernel stack segment.
    tss.esp0 = esp0; // Set the kernel stack pointer.

    // Here we set the cs, ss, ds, es, fs and gs entries in the TSS. These specify what
    // segments should be loaded when the processor switches to kernel mode. Therefore
    // they are just our normal kernel code/data segments - 0x08 and 0x10 respectively,
    // but with the last two bits set, making 0x0b and 0x13. The setting of these bits
    // sets the RPL (requested privilege level) to 3, meaning that this TSS can be used
    // to switch to kernel mode from ring 3.
    tss.cs   = 0x0b;
    tss.ss = tss.ds = tss.es = tss.fs = tss.gs = 0x13;
}

void Arch::gdt_install()
{
    //update pointer
    gp.size = (sizeof(struct gdt_entry) * I86_GDT_ENTRY_COUNT) - 1;
	gp.offset = (uint32_t)&gdt;
	
	//load gdt
    gdt_flush();
    tss_flush();
}

void Arch::gdt_set_kernel_stack(uint32_t esp0)
{
    tss.esp0 = esp0;
}
