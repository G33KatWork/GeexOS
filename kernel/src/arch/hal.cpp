#include <arch/hal.h>
#include <arch/gdt.h>
#include <arch/idt.h>
#include <arch/pit.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <arch/Paging.h>
#include <arch/AddressLayout.h>
#include <kernel/debug.h>
#include <arch/cpuid.h>
#include <arch/msr.h>
#include <arch/ACPI/RSDP.h>
#include <arch/ACPI/RSDT.h>

using namespace Arch;
using namespace Memory;
using namespace ACPI;
using namespace IO;

void Arch::InitializeCPU()
{
    ASSERT(CPUhasMSR(), "CPU doesn't seem to support MSRs. It also looks a bit rusty. What do you think about getting a slightly newer one?");
    
    Paging::GetInstance()->Init();
    HAL_DEBUG_MSG("Paging initialized...");
    
    gdt_setup();
    gdt_install();
    HAL_DEBUG_MSG("GDT installed...");
    
    idt_setup();
    idt_install();
    HAL_DEBUG_MSG("IDT installed...");
    
    InitializePIC();
    
    //Set WP (Write Protect) bit in CR0
    //This causes a page fault when writing to read-only pages even if we are in kernel mode
    asm volatile (
		"mov %cr0, %eax\n"
		"orl $0x10000, %eax\n"
		"mov %eax, %cr0\n");
}

void Arch::InitDone()
{
    //Remove lower 4MB mapping, we are fully at 3GB now
    Paging::GetInstance()->InitDone();
    
    HAL_DEBUG_MSG("Arch initialization done...");
}

void Arch::GetCPUVendor(char* buf)
{
    cpuid_string(CPUID_GETVENDORSTRING, (uint32_t*)buf);
}

void Arch::SetupArchMemRegions()
{
    HAL_DEBUG_MSG("Announcing architecture specific memory regions...");
    
    //Announce BIOS region
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegion(BIOS_ADDRESS, BIOS_SIZE, "BIOS", ALLOCFLAG_WRITABLE);
    
    //Announce region for video memory
    //TODO: Implement proper framebuffer, configure VGA properly and throw this away
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegion(KERNEL_VIDEO_MEMORY, 2*PAGE_SIZE, "Video memory", ALLOCFLAG_WRITABLE);
    
    HAL_DEBUG_MSG("Setting up ACPI...");
    
    RSDP* p = new RSDP();
    
    if(p->Found())
    {
        HAL_DEBUG_MSG("ACPI seems to be available...");
        ASSERT(p->IsValid(), "RSDP is invalid");
    }
    
    HAL_DEBUG_MSG("ACPI OEM ID: " << p->GetOEMID());
    
    Address rsdtAddr = p->GetRSDTAddress();
    HAL_DEBUG_MSG("RSDT Address: " << rsdtAddr);
    
    //TODO: Map to another address? Perhaps a fixed one in kernel space. Or just parse everything and throw this away
    VirtualMemoryManager::GetInstance()->KernelSpace()->MapPhysical(rsdtAddr, rsdtAddr, PAGE_SIZE, "ACPI Tables", ALLOCFLAG_NONE);
    
    RSDT* rsdt = RSDT::FromAddress(rsdtAddr);
    ASSERT(rsdt->IsValid(), "Checksum of RSDT is invalid");
    
    HAL_DEBUG_MSG("RSDT contains " << dec << rsdt->GetSubtableCount() << " Subtables");
    for(unsigned int i = 0; i < rsdt->GetSubtableCount(); i++)
    {
        Address a = (Address)rsdt->GetTable(i);
        ACPITableHeader* table = (ACPITableHeader*)a;
        HAL_DEBUG_MSG("Address of RSDT Subtable " << dec << i << ": " << hex << a);
        //Signatures are not null-terminated, anyway we just print this here 
        HAL_DEBUG_MSG("Subtable Signature: " << table->Signature);
    }
}

ClockSource_t Arch::ClockSource  = {
    "PIT",
    1000000,  //ticklength in ns: 1000ms = 1000000ns
    PERIODIC,
    NULL,
    PIT::Enable,
    PIT::Disable
};
