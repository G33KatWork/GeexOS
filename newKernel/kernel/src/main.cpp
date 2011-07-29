#include <types.h>
#include <limits.h>

#include <halinterface/HAL.h>

#include <kernel/debug.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <kernel/Memory/Virtual/Regions/KernelProgramMemoryRegion.h>

#include <kernel/Modules/KernelModule.h>

using namespace Arch;
using namespace Debug;
using namespace Memory;
using namespace Modules;

int main()
{
    #ifdef SERIAL_DEBUG
        CurrentHAL->SetCurrentDebugOutputDeviceType(Debug::Serial);
    #endif
    CurrentHAL->GetCurrentDebugOutputDevice()->Clear();
    
    CurrentHAL->Initialize();
    
    MAIN_DEBUG_MSG("Kernel commandline: " << CurrentHAL->GetBootEnvironment()->GetKernelCommandline());
    
    //Initialize Memory
    VirtualMemoryManager::GetInstance()->Init();
    MAIN_DEBUG_MSG("Virtual memory manager instace created...");
    
    //Build virtual memory space for kernel ELF
    VirtualMemoryManager::GetInstance()->KernelSpace(
        new VirtualMemorySpace(VirtualMemoryManager::GetInstance(), "KernelSpace", CurrentHAL->GetPaging()->GetKernelDirectory())
    );
    VirtualMemoryManager::GetInstance()->SetCurrentMemorySpace(VirtualMemoryManager::GetInstance()->KernelSpace()); //Just to be sure...
    MAIN_DEBUG_MSG("Virtual memory space for kernel created...");
    
    CurrentHAL->InitializationDone();
    
    MAIN_DEBUG_MSG("Telling memory manager about kernel program regions...");
    size_t progRegionCount = CurrentHAL->GetBootEnvironment()->GetProgramRegionCount();
    for(size_t i = 0; i < progRegionCount; i++)
    {
        KernelProgramRegion* regionInfo = CurrentHAL->GetBootEnvironment()->GetProgramRegion(i);
        KernelProgramMemoryRegion* progRegion = new KernelProgramMemoryRegion(
                                                        regionInfo->VirtualStart,
                                                        regionInfo->Length,
                                                        "Kernel Program",
                                                        (regionInfo->Writable ? ALLOCFLAG_WRITABLE : 0) | (regionInfo->Executable ? ALLOCFLAG_EXECUTABLE : 0)
                                                    );
        VirtualMemoryManager::GetInstance()->KernelSpace()->AddRegion(progRegion);
    }
    
    //Yeeha! At this point all platform related memory stuff should be set up and safe.
    
    //TODO: Implement normal (non-debug) output-macro that prints text to graphical screen via HAL
    //which gets initialized after the HAL INIT is finished
    MAIN_DEBUG_MSG("GeexOS Kernel booting...");
    
    MAIN_DEBUG_MSG("Physical memory manager has the following addresses marked as used:");
#ifdef EN_DEBUG_MSG_MAIN
    CurrentHAL->GetPhysicalMemoryAllocator()->DumpUsed(CurrentHAL->GetCurrentDebugOutputDevice());
#endif
    
    MAIN_DEBUG_MSG("IOMemoryManager knows the following regions:");
#ifdef EN_DEBUG_MSG_MAIN
    VirtualMemoryManager::GetInstance()->IOMemory()->DumpRegions(CurrentHAL->GetCurrentDebugOutputDevice());
#endif

    MAIN_DEBUG_MSG("Virtual kernel space knows the following regions:");
#ifdef EN_DEBUG_MSG_MAIN
    VirtualMemoryManager::GetInstance()->KernelSpace()->DumpRegions(CurrentHAL->GetCurrentDebugOutputDevice());
#endif

    MAIN_DEBUG_MSG("Module BLOB is at " << hex << CurrentHAL->GetBootEnvironment()->GetModuleLocation() << " with size " << CurrentHAL->GetBootEnvironment()->GetModuleSize());
    
    KernelModule* obj = GXAllocateFromType(KernelModule);
    obj->Plug();
    
    while(1);
}
