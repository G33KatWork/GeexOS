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
#include <arch/ACPI/FADT.h>
#include <arch/ACPI/AmlDST.h>
#include <arch/ACPI/HPET.h>

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

void Arch::SetupArchMemRegions(Multiboot* m)
{
    HAL_DEBUG_MSG("Announcing architecture specific memory regions...");
    
    //Announce BIOS region
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegion(BIOS_ADDRESS, BIOS_SIZE, "BIOS", ALLOCFLAG_WRITABLE);
    
    //Announce region for video memory
    //TODO: Implement proper framebuffer, configure VGA properly and throw this away
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegion(KERNEL_VIDEO_MEMORY, 2*PAGE_SIZE, "Video memory", ALLOCFLAG_WRITABLE);
    
    //Reserve memory regions
    HAL_DEBUG_MSG("The following " << dec << m->GetMemregionCount() << " memory regions were given from the bootloader:");
    HAL_DEBUG_MSG("Address\t\tLength\t\tType");
    for(unsigned int i = 0; i < m->GetMemregionCount(); i++)
    {
        multiboot_memory_region_t region = m->GetMemregions()[i];
        HAL_DEBUG_MSG("" << hex << (Address)region.addr << "\t\t" << (Address)region.len << "\t\t" << region.type);

        //check if we need to reserve this region
        //that's the case if the type is not 0x1 and it's not the bios region which we resevered above
        if(region.type != 0x1 && region.addr > ((BIOS_ADDRESS + BIOS_SIZE) - KERNEL_BASE))
        {
            Address curAddress = (Address)region.addr;
            Address curLength = (Address)region.len;
            HAL_DEBUG_MSG("Reserving area at " << hex << curAddress);
            while(curLength != 0)
            {
                VirtualMemoryManager::GetInstance()->PhysicalAllocator()->MarkAsUsed(curAddress);
                curAddress += PAGE_SIZE;
                curLength -= PAGE_SIZE;
            }
        }
    }
    
    
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
    
    FADT* fadt = FADT::FromAddress(rsdt->GetTable("FACP"));
    if(fadt != NULL)
    {
        HAL_DEBUG_MSG("Found FADT");
        ASSERT(fadt->IsValid(), "FADT is invalid");
        HAL_DEBUG_MSG("FACS Address: " << hex << fadt->GetFACSAddress());
        HAL_DEBUG_MSG("DSDT Address: " << hex << fadt->GetDSDTAddress());
        
        if(fadt->GetFACSAddress() != NULL)
        {
            HAL_DEBUG_MSG("Found FACS");
            //TODO: Write class for FACS
        }
        
        AmlSDT* dsdt = AmlSDT::FromAddress(fadt->GetDSDTAddress());
        ASSERT(dsdt->IsValid(), "DSDT is invalid");
        HAL_DEBUG_MSG("DSDT contains " << dec << dsdt->GetAMLCodeLength() << " Byte AML Code");
    }
    
    AmlSDT* ssdt = AmlSDT::FromAddress(rsdt->GetTable("SSDT"));
    if(ssdt != NULL)
    {
        HAL_DEBUG_MSG("Found SSDT");
        ASSERT(ssdt->IsValid(), "SSDT is invalid");
        HAL_DEBUG_MSG("SSDT contains " << dec << ssdt->GetAMLCodeLength() << " Byte AML Code");
    }
    
    HPET* hpet = HPET::FromAddress(rsdt->GetTable("HPET"));
    if(hpet != NULL)
    {
        HAL_DEBUG_MSG("Found HPET");
        ASSERT(hpet->IsValid(), "HPET is invalid");
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
