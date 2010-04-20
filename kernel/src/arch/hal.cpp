#include <arch/hal.h>
#include <arch/gdt.h>
#include <arch/idt.h>
#include <arch/pit.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <arch/Paging.h>

using namespace Arch;
using namespace Memory;

void Arch::InitializeCPU()
{
    Paging::GetInstance()->Init();
    
    gdt_setup();
    gdt_install();
    
    idt_install();
    
    //Set WP (Write Protect) bit in CR0
    //This causes a page fault when writing to read-only pages even if we are in kernel mode
    asm volatile (
		"mov %cr0, %eax\n"
		"orl $0x10000, %eax\n"
		"mov %eax, %cr0\n");
}

void Arch::SetupArchMemRegions()
{
    //Announce BIOS region
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegion(BIOS_ADDRESS, BIOS_SIZE, "BIOS", ALLOCFLAG_WRITABLE);
    
    //Announce region for video memory
    //TODO: Implement proper framebuffer, configure VGA properly and throw this away
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegion(0xB8000, 2*PAGE_SIZE, "Video memory", ALLOCFLAG_WRITABLE);
}

ClockSource_t Arch::ClockSource  = {
    "PIT",
    1000000,  //ticklength in ns: 1000ms = 1000000ns
    PERIODIC,
    NULL,
    PIT::Enable,
    PIT::Disable
};
