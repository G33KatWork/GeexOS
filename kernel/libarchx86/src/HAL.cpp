#include <arch/HAL.h>
#include <kernel/global.h>
#include <kernel/debug.h>
#include <arch/internal/CPUID.h>
#include <arch/internal/MSR.h>
#include <arch/internal/GDT.h>
#include <arch/internal/IDT.h>
#include <arch/internal/PIC.h>
#include <arch/x86Paging.h>
#include <arch/x86InterruptDispatcher.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <arch/AddressLayout.h>
#include <arch/ExceptionHandler/FatalExceptionHandler.h>
#include <arch/ExceptionHandler/PageFaultHandler.h>
#include <arch/x86BootEnvironment.h>
#include <kernel/Memory/IO/IOMemoryManager.h>

#include <arch/VBEDebugOutput.h>

using namespace Arch;
using namespace Arch::ACPI;
using namespace Memory;
using namespace Debug;
using namespace Memory;

//Create an instance of our HAL for easy access from anywhere
x86HAL aNewHAL = x86HAL();
HAL* Arch::CurrentHAL = &aNewHAL;

//defined and set on start.S
extern MultibootInfo* multibootLocation;

x86HAL::x86HAL()
{
    ird = NULL;
    paging = NULL;
    bootenv = NULL;
    serialDebug = NULL;
    textDebug = NULL;
    graphicalDebug = NULL;
    pit = NULL;
    pic = NULL;
    ioapic = NULL;
    nullDebug = NullDebugOutputDevice();
    currentDebugDevice = None;
    bootstrapProcessor = NULL;
}

void x86HAL::Initialize()
{
    HAL_DEBUG_MSG("x86 HAL Hardware initialization started...");
    ASSERT(CPUhasMSR(), "CPU doesn't seem to support MSRs. It also looks a bit rusty. What do you think about getting a slightly newer one?");
    
    GetPaging()->Init();
    HAL_DEBUG_MSG("Paging initialized...");
    
    gdt_setup();
    gdt_install();
    HAL_DEBUG_MSG("GDT installed...");
    
    if(multibootLocation == NULL)
        PANIC("The multiboot structure pointer is NULL. Not good!");
    bootenv = new x86BootEnvironment(multibootLocation);
    HAL_DEBUG_MSG("Boot environment information successfully parsed...");
    
    idt_setup();
    idt_install();
    HAL_DEBUG_MSG("IDT installed...");
    
    ird = new x86InterruptDispatcher();
    FatalExceptionHandler* fatalHandler = new FatalExceptionHandler();
    ird->RegisterExceptionHandler(BaseInterruptDispatcher::EXC_DIVBY0, fatalHandler);
    ird->RegisterExceptionHandler(BaseInterruptDispatcher::EXC_NMI, fatalHandler);
    ird->RegisterExceptionHandler(BaseInterruptDispatcher::EXC_OVERFLOW, fatalHandler);
    ird->RegisterExceptionHandler(BaseInterruptDispatcher::EXC_BOUNDCHECK, fatalHandler);
    ird->RegisterExceptionHandler(BaseInterruptDispatcher::EXC_INVALIDOPCODE, fatalHandler);
    ird->RegisterExceptionHandler(BaseInterruptDispatcher::EXC_COPROCNOTAVAIL, fatalHandler);
    ird->RegisterExceptionHandler(BaseInterruptDispatcher::EXC_DOUBLEFAULT, fatalHandler);
    ird->RegisterExceptionHandler(BaseInterruptDispatcher::EXC_COPROCSEGMENTOVERRUN, fatalHandler);
    ird->RegisterExceptionHandler(BaseInterruptDispatcher::EXC_INVALIDTSS, fatalHandler);
    ird->RegisterExceptionHandler(BaseInterruptDispatcher::EXC_SEGMENTNOTPRESENT, fatalHandler);
    ird->RegisterExceptionHandler(BaseInterruptDispatcher::EXC_STACK, fatalHandler);
    ird->RegisterExceptionHandler(BaseInterruptDispatcher::EXC_GENERALPROTECTIONFAULT, fatalHandler);
    ird->RegisterExceptionHandler(BaseInterruptDispatcher::EXC_COPROCERROR, fatalHandler);
    ird->RegisterExceptionHandler(BaseInterruptDispatcher::EXC_PAGEFAULT, new PageFaultHandler());
    HAL_DEBUG_MSG("Interrupt dispatcher initialized...");
    
    //Set WP (Write Protect) bit in CR0
    //This causes a page fault when writing to read-only pages even if we are in kernel mode
    asm volatile (
		"mov %cr0, %eax\n"
		"orl $0x10000, %eax\n"
		"mov %eax, %cr0\n");
	
	HAL_DEBUG_MSG("x86 Hardware initialization done...");
}

void x86HAL::InitializationDone()
{
    //Remove lower 4MB mapping, we are fully at 3GB now
    GetPaging()->InitDone();
    
    HAL_DEBUG_MSG("Arch initialization done...");

    /*Debug::VBEDebugOutput* vbe = new Debug::VBEDebugOutput();
    vbe->Test();*/ 
}

void x86HAL::InitializationAfterMemoryAvailable()
{
    HAL_DEBUG_MSG("Arch initialization after memory initialization...");
    
    acpiParser = new ACPIParser();
    acpiParser->Parse();
    
    //Configure LAPIC on bootstrap processor
    //FIXME: Use ACPI tables for LAPICs
    if(LAPIC::IsAvailable())
    {
        HAL_DEBUG_MSG("LAPIC is available. Configuring...");
        
        LAPIC* lapic = new LAPIC();
        
        // Map LAPIC into IO Memory
        lapic->MapIntoIOMemory();
        
        lapic->Initialize();
        bootstrapProcessor = new x86Processor(lapic->GetID(), lapic);
        
        lapic->DetermineBusFrequency();
        
        lapic->SetTickLengthUs(1000);
        HAL_DEBUG_MSG("LAPIC with ID " << Debug::dec << lapic->GetID() << " on bootstrapping processor initialized...");
    }
    else
    {
        HAL_DEBUG_MSG("LAPIC is not available. Falling back to PIT Timer.");
        bootstrapProcessor = new x86Processor(0, NULL);
        
        pit = new PIT();
        pit->SetTickLengthUs(1000);     //1000us = 1ms
        HAL_DEBUG_MSG("PIT initialized...");
    }
    
    //FIXME: Handle more than 1 I/O APIC
    IOAPICStructure* ioapicStruct = (IOAPICStructure*)acpiParser->GetMADT()->GetAPICStructOfType(0, MADT_IOAPIC);
    
    //Setup IOAPIC or PIC
    if(ioapicStruct)
    {
        HAL_DEBUG_MSG("IOAPIC Structure found in ACPI, configuring...");
        HAL_DEBUG_MSG("IOAPIC ID is " << hex << ioapicStruct->IOAPICID);
        HAL_DEBUG_MSG("IOAPIC Base address is " << hex << ioapicStruct->IOAPICAddress);
        HAL_DEBUG_MSG("IOAPIC global system interrupt base is " << hex << ioapicStruct->GlobalSystemInterruptBase);
        
        ioapic = new IOAPIC(ioapicStruct->IOAPICAddress, ioapicStruct->GlobalSystemInterruptBase);
        ioapic->MapIntoIOMemory();
        
        if(ioapic->GetID() != ioapicStruct->IOAPICID)
            ioapic->SetID(ioapicStruct->IOAPICID);
        ASSERT(ioapic->GetID() == ioapicStruct->IOAPICID, "IOAPIC id is not equal to IOAPIC id in MADT ACPI Table");
        ioapic->Initialize();
        
        //configure interrupt source overrides for IOAPIC
        InterruptSourceOverrideStructure* override = (InterruptSourceOverrideStructure*)acpiParser->GetMADT()->GetAPICStructOfType(0, MADT_INTERRUPTSOURCEOVERRIDE);
        while(override)
        {
            HAL_DEBUG_MSG("Found interrupt source override from " << dec << override->Source << " to " << override->GlobalSystemInterrupt);
            override = (InterruptSourceOverrideStructure*)acpiParser->GetMADT()->GetNextAPICStructOfType((APICStructureHeader*)override, MADT_INTERRUPTSOURCEOVERRIDE);
        }
        
        HAL_DEBUG_MSG("Configured IOAPIC with id " << hex << ioapic->GetID() << " in version " << ioapic->GetVersion());
    }
    //else
    {
        HAL_DEBUG_MSG("No IOAPIC Structure found in ACPI, falling back to legacy PIC...");
        pic = new PIC();
        pic->Initialize();
    }
}

void x86HAL::SetupArchMemRegions()
{
    HAL_DEBUG_MSG("Announcing architecture specific memory regions...");
    
    //Announce BIOS region
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegionWithPreallocatedMemory(
                                                        BIOS_ADDRESS,
                                                        BIOS_SIZE,
                                                        "BIOS",
                                                        ALLOCFLAG_WRITABLE);
    
    //Announce region for video memory
    /*VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegionWithPreallocatedMemory(
                                                        KERNEL_VIDEO_MEMORY,
                                                        2*PAGE_SIZE,
                                                        "Video memory",
                                                        ALLOCFLAG_WRITABLE);*/
    
    x86BootEnvironment* m = (x86BootEnvironment*)GetBootEnvironment();
    
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

            IOMemoryRegion* iomemregion = VirtualMemoryManager::GetInstance()->IOMemory()->MapPhysical(region.addr, region.len, "MMAP Reserved");
            HAL_DEBUG_MSG("Reserved region mapped to virtual " << hex << iomemregion->StartAddress());
        }
    }
}
    
void x86HAL::EnableInterrupts()
{
    asm volatile("sti");
}

void x86HAL::DisableInterrupts()
{
    asm volatile("cli");
}
    
void x86HAL::GetCPUVendor(char* buf)
{
    cpuid_string(CPUID_GETVENDORSTRING, (uint32_t*)buf);
}

void x86HAL::HaltMachine()
{
    DisableInterrupts();
    asm volatile("hlt");
}

BaseInterruptDispatcher* x86HAL::GetInterruptDispatcher()
{
    return ird;
}

BasePaging* x86HAL::GetPaging()
{
    if(paging == NULL)
        paging = new x86Paging();
    
    return paging;
}

BaseTimer* x86HAL::GetHardwareClockSource()
{
    if(((x86Processor*)GetCurrentProcessor())->GetLAPIC() != NULL)
        return ((x86Processor*)GetCurrentProcessor())->GetLAPIC();
        
    return pit;
}

Debug::BaseDebugOutputDevice* x86HAL::GetCurrentDebugOutputDevice()
{
    switch(currentDebugDevice)
    {
        case TextMonitor:
            return textDebug;
        case GraphicalMonitor:
            return graphicalDebug;
        case Serial:
            return serialDebug;
        default:
            return &nullDebug;
    }
}

void x86HAL::SetCurrentDebugOutputDeviceType(Debug::DebugOutputDeviceType type)
{
    switch(type)
    {
        case TextMonitor:
            if(textDebug == NULL)
                textDebug = new TextmodeDebugOutput();
            break;
        case GraphicalMonitor:
            //if(graphicalDebug == NULL)
            //    graphicalDebug = new VBEDebugOutput();
            break;
        case Serial:
            if(serialDebug == NULL)
                serialDebug = new SerialDebugOutput(SERIAL_COM1);
            break;
        case None:
            break;
    }
    
    currentDebugDevice = type;
}
