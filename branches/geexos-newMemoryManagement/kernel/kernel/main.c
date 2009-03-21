#include "arch/x86/gdt.h"
#include "arch/x86/idt.h"
#include "arch/x86/paging.h"
#include "arch/x86/utils.h"
#include "arch/x86/pit.h"
#include "arch/x86/isr.h"
#include <bootinfo.h>
#include "DebugDisplay.h"
#include "MemoryManager.h"
#include "FrameAllocator.h"
#include "panic.h"

int kmain (struct multiboot_info* bootinfo);
void page_fault(registers_t regs);

int kmain (struct multiboot_info* bootinfo)
{	
	init_paging();
	register_interrupt_handler(14, &page_fault);
	gdt_install();
	paging_remove_lowest4MB();
	idt_install();
	init_pit(50);
	
	DebugClrScr (0x18);
	DebugSetColor (0x19);
	
	//correct pointers after paging works...
	bootinfo = (struct multiboot_info*)((char *)bootinfo + 0xC0000000);
	bootinfo->cmdline = bootinfo->cmdline + 0xC0000000;
	bootinfo->mods_addr = bootinfo->mods_addr + 0xC0000000;
	bootinfo->syms3 = bootinfo->syms3 + 0xC0000000;
	bootinfo->mmap_addr = bootinfo->mmap_addr + 0xC0000000;
	bootinfo->drives_addr = bootinfo->drives_addr + 0xC0000000;
	bootinfo->apm_table = bootinfo->apm_table + 0xC0000000;
	bootinfo->boot_loader_name = bootinfo->boot_loader_name + 0xC0000000;
	
	uint32_t memSize = bootinfo->mem_lower + bootinfo->mem_upper;
	uint32_t multibootFlags = bootinfo->flags;

	DebugGotoXY (0,0);
	DebugSetColor (0x70);
	DebugPrintf (" GeexOs Kernel preparing to load...                                             ");
	DebugGotoXY (0,1);
	DebugSetColor (0x19);
	DebugPrintf (" GeexOS Starting Up...\n");
	DebugPrintf (" Installed memory: %iKB\n", memSize);
	DebugPrintf (" Multiboot flags: %x\n", multibootFlags);
	DebugPrintf (" Kernel commandline: %s\n", (const char*)(bootinfo->cmdline));
	DebugPrintf (" Processor vendor: %s\n\n", get_cpu_vendor());
	
	//Test frame allocator
	init_allocator(memSize);
	uint32_t* a = allocate_frame();
	uint32_t* b = allocate_frame();
	DebugPrintf (" Allocated frame a: %x\n", a);
	DebugPrintf (" Allocated frame b: %x\n", b);
	DebugPrintf (" Freeing frame b...\n");
	free_frame(b);
	uint32_t* c = allocate_frame();
	DebugPrintf (" Allocated frame c: %x\n\n", c);
	
	//Test frame mapping
	paging_map_address(0x100000, 0x1000000, 0x3);
	uint8_t* x = (uint8_t*)0x1000000;
	*(x+0xFFF) = 0x10;
	DebugPrintf (" virtual: 0x1001000 physical: 0x101000: %x\n\n", *(x+0xFFF));
	
	asm volatile ("sti");
	asm volatile ("int $0x3");
	asm volatile ("int $0x4");

	DebugGotoXY (0,20);
	DebugPrintf(" Ticks: ");
	for(;;)
	{
		DebugGotoXY (8,20);
		DebugPrintf ("%u", get_ticks());
	}

	return 0;
}

void page_fault(registers_t regs)
{
    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
    
    // The error code gives us details of what happened.
    int present   = !(regs.err_code & 0x1); // Page not present
    int rw = regs.err_code & 0x2;           // Write operation?
    int us = regs.err_code & 0x4;           // Processor was in user-mode?
    int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    int id = regs.err_code & 0x10;          // Caused by an instruction fetch?

    kernel_panic("Page fault! ( %s%s%s%s%s) at 0x%x",
		(present ? "present " : ""),
		(rw ? "read-only " : ""),
		(us ? "user-mode " : ""),
		(reserved ? "reserved " : ""),
		(id ? "instruction-fetch " : ""),
		faulting_address
	);
}