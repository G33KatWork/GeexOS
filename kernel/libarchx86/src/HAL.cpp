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
    nullDebug = NullDebugOutputDevice();
    currentDebugDevice = None;
    
    //can't assign directly to clk, why?
    ClockSource clk_tmp = {
        "PIT",
        1000000,  //ticklength in us //FIXME: timer frequency is wrong
        CLKTYPE_PERIODIC,
        NULL,
        PIT::Enable,
        PIT::Disable
    };
    clk = clk_tmp;
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
    
    InitializePIC();
    HAL_DEBUG_MSG("PIC initialized...");
    
    pit = new PIT(1000); //1000Hz = 1ms
    HAL_DEBUG_MSG("PIT initialized...");
    
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

    Debug::VBEDebugOutput* vbe = new Debug::VBEDebugOutput();
    vbe->Test();
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

ClockSource* x86HAL::GetHardwareClockSource() 
{
    return &clk;
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
