#include <arch/HAL.h>
#include <arch/x86Paging.h>
#include <arch/x86BootEnvironment.h>
#include <arch/x86InterruptDispatcher.h>

#include <arch/internal/GDT.h>
#include <arch/internal/IDT.h>
#include <arch/internal/CPUID.h>
#include <arch/internal/MSR.h>
#include <arch/internal/x86PageFaultHandler.h>

#include <kernel/debug.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>

using namespace Arch;
using namespace Debug;
using namespace Memory;

//Create an instance of our HAL for easy access from anywhere
x86HAL aNewHAL = x86HAL();
HAL* Arch::CurrentHAL = &aNewHAL;

//defined in start.S
extern KernelInformation* kernelInformation;

x86HAL::x86HAL()
{
    bootenv = NULL;
    serialDebug = NULL;
    physicalAllocator = NULL;
    paging = NULL;
    interruptDispatcher = NULL;
    nullDebug = NullDebugOutputDevice();
    currentDebugDevice = None;
    pageFaultHandler = NULL;
    halFullyInitialized = false;
}

void x86HAL::Initialize()
{
    HAL_DEBUG_MSG("x86 HAL Hardware initialization started...");
    ASSERT(CPUhasMSR(), "CPU doesn't seem to support MSRs. It also looks a bit rusty. What do you think about getting a slightly newer one?");
    
    gdt_setup();
    gdt_install();
    HAL_DEBUG_MSG("GDT installed...");
    
    idt_setup();
    idt_install();
    HAL_DEBUG_MSG("IDT installed...");
    
    if(kernelInformation == NULL)
        PANIC("The multiboot structure pointer is NULL. Not good!");
    
    HAL_DEBUG_MSG("Parsing boot environment structure...");
    bootenv = new x86BootEnvironment(kernelInformation);
    HAL_DEBUG_MSG("Boot environment information successfully parsed...");
    
    physicalAllocator = new BitfieldPhysicalMemoryAllocator(bootenv->GetInstalledMemory());

    //Mark all memory regions allocated by the bootloader of importance for us as allocated in the physical allocator
    size_t descriptorCount;
    PMEMORY_DESCRIPTOR descriptor = ((x86BootEnvironment*)bootenv)->GetMemoryRegions(&descriptorCount);
    for(size_t i = 0; i < descriptorCount; i++)
    {
        switch(descriptor[i].Type)
        {
            case MemoryTypeBad:
            case MemoryTypeSpecial:
            case MemoryTypeFirmware:
            case MemoryTypeGeexOSKernelEnvironmentInformation:
            case MemoryTypeGeexOSKernelExecutable:
            case MemoryTypeGeexOSKernelStack:
            case MemoryTypeGeexOSKernelLibrary:
                for(Address a = descriptor[i].Start; a < descriptor[i].Start + descriptor[i].Length; a++)
                    physicalAllocator->MarkAsUsed(a * PAGE_SIZE);

            //Don't mark these regions as allocated as the paging code will reallocate them anyways in x86Paging::Init()
            case MemoryTypeGeexOSPageStructures:
            case MemoryTypeGeexOSPageDirectory:
            default:
                break;
        }
    }
    
    interruptDispatcher = new x86InterruptDispatcher();
    pageFaultHandler = new x86PageFaultHandler();
    interruptDispatcher->RegisterInterruptVector(14, pageFaultHandler);
    
    GetPaging()->Init();
    
    //Set WP (Write Protect) bit in CR0
    //This causes a page fault when writing to read-only pages even if we are in kernel mode
    asm volatile (
		"mov %cr0, %eax\n"
		"orl $0x10000, %eax\n"
		"mov %eax, %cr0\n");
	
	HAL_DEBUG_MSG("x86 Hardware initialization done...");
}

void x86HAL::reserveBIOSMemregions()
{
    /*HAL_DEBUG_MSG("The following " << dec << ((x86BootEnvironment*)bootenv)->GetBIOSMemregionCount() << " memory regions were given from the bootloader:");
    HAL_DEBUG_MSG("Address\t\tLength\t\tType");
    for(unsigned int i = 0; i < ((x86BootEnvironment*)bootenv)->GetBIOSMemregionCount(); i++)
    {
        KernelInformationMemoryRegion* region = ((x86BootEnvironment*)bootenv)->GetBIOSMemregion(i);
        HAL_DEBUG_MSG("" << hex << (Address)region->addr << "\t\t" << (Address)region->len << "\t\t" << region->type);

        if(region->type != 0x1)
        {
            Address curAddress = (Address)(region->addr & PAGEALIGN_MASK);
            size_t length = IS_PAGE_ALIGNED(region->len) ? region->len : PAGE_ALIGN(region->len);
            
            HAL_DEBUG_MSG("Reserving area at " << hex << curAddress << " with length " << length);
            
            while(length != 0)
            {
                this->physicalAllocator->MarkAsUsed(curAddress);
                
                curAddress += PAGE_SIZE;
                length -= PAGE_SIZE;
            }

            length = IS_PAGE_ALIGNED(region->len) ? region->len : PAGE_ALIGN(region->len);

            IOMemoryRegion* iomemregion = VirtualMemoryManager::GetInstance()->IOMemory()->MapPhysical(region->addr & PAGEALIGN_MASK, length, "BIOS Reserved");
            HAL_DEBUG_MSG("Reserved region mapped to virtual " << hex << iomemregion->StartAddress());
        }
    }*/
}

void x86HAL::InitializationDone()
{
    reserveBIOSMemregions();
    
    //FIXME: takes so much RAM...
    GetPaging()->InitDone();
    
    halFullyInitialized = true;
    HAL_DEBUG_MSG("Arch initialization done...");
    
    //Now, that we have IO Memory management, we can use some more sophisticated text output
    initializeBootGraphics();
}

void x86HAL::initializeBootGraphics()
{
    //TODO: do vbe stuff here
}
    
void x86HAL::EnableInterruptsOnCurrentCPU()
{
    asm volatile("sti");
}

void x86HAL::DisableInterruptsOnCurrentCPU()
{
    asm volatile("cli");
}
    
void x86HAL::GetCPUVendorName(char* buf)
{
    cpuid_string(CPUID_GETVENDORSTRING, (uint32_t*)buf);
}

void x86HAL::HaltCurrentCPU()
{
    DisableInterruptsOnCurrentCPU();
    asm volatile("hlt");
}

BasePaging* x86HAL::GetPaging()
{
    if(paging == NULL)
        paging = new x86Paging();
    
    return paging;
}

Debug::BaseOutputDevice* x86HAL::GetCurrentDebugOutputDevice()
{
    switch(currentDebugDevice)
    {
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
        case Serial:
            if(serialDebug == NULL)
                serialDebug = new SerialDebugOutput(SERIAL_COM1);
            break;
        case None:
            break;
    }
    
    currentDebugDevice = type;
}

BasePhysicalMemoryAllocator* x86HAL::GetPhysicalMemoryAllocator()
{
    return physicalAllocator;
}

BaseInterruptDispatcher* x86HAL::GetInterruptDispatcher()
{
    return interruptDispatcher;
}
