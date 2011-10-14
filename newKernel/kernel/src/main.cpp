#include <types.h>
#include <limits.h>

#include <halinterface/HAL.h>

#include <kernel/debug.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <kernel/Memory/Virtual/Regions/KernelProgramMemoryRegion.h>
#include <kernel/Memory/Slab/SlabAllocator.h>

#include <kernel/Modules/KernelModule.h>

#include <string.h>

using namespace Arch;
using namespace Debug;
using namespace Memory;
using namespace Memory::Slab;
using namespace Modules;

#include <kernel/Objects/GXObject.h>

class TestObject : public GXObject
{
    GXDeclareDefaultMetadata(TestObject)
    
public:
    void doSomething()
    {
        MAIN_DEBUG_MSG("TestObject did something");
    }
};

GXImplementMetaClassAndDefaultStructors(TestObject, GXObject)

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
    
    //Initialize virtual memory management
    VirtualMemoryManager::GetInstance()->KernelSpace(
        new VirtualMemorySpace(VirtualMemoryManager::GetInstance(), "KernelSpace", CurrentHAL->GetPaging()->GetKernelDirectory())
    );
    VirtualMemoryManager::GetInstance()->SetCurrentMemorySpace(VirtualMemoryManager::GetInstance()->KernelSpace()); //Just to be sure...
    MAIN_DEBUG_MSG("Virtual memory space for kernel created...");
    
    //Tell HAL that we have a virtual memory manager
    CurrentHAL->InitializationDone();
    
    //Reserve ELF program regions of kernel in virtual memory management
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
    
    //Initialize dynamic memory with the slab allocator
    SlabAllocator* slabAlloc = new SlabAllocator(KERNEL_SLAB_ALLOCATOR_START, KERNEL_SLAB_ALLOCATOR_SIZE);
    VirtualMemoryManager::GetInstance()->KernelSpace()->AddRegion(slabAlloc);
    InitializeSizeCaches(slabAlloc);
    
    //Yeeha! At this point all platform related memory stuff should be set up and safe.
    
    //TODO: Implement normal (non-debug) output-macro that prints text to graphical screen via HAL
    //which gets initialized after the HAL INIT is finished
    MAIN_DEBUG_MSG("GeexOS Kernel booting...");

    //Create module repository memory region for boot-critical modules
    MAIN_DEBUG_MSG("Module BLOB is at " << hex << (Address)CurrentHAL->GetBootEnvironment()->GetBootModuleRepository() << " with size " << CurrentHAL->GetBootEnvironment()->GetBootModuleRepositorySize());


#if 0
#ifdef EN_DEBUG_MSG_MAIN
    // MAIN_DEBUG_MSG("Physical memory manager has the following addresses marked as used:");
    // CurrentHAL->GetPhysicalMemoryAllocator()->DumpUsed(CurrentHAL->GetCurrentDebugOutputDevice());
    
    MAIN_DEBUG_MSG("IOMemoryManager knows the following regions:");
    VirtualMemoryManager::GetInstance()->IOMemory()->DumpRegions(CurrentHAL->GetCurrentDebugOutputDevice());

    MAIN_DEBUG_MSG("Virtual kernel space knows the following regions:");
    VirtualMemoryManager::GetInstance()->KernelSpace()->DumpRegions(CurrentHAL->GetCurrentDebugOutputDevice());
    
    MAIN_DEBUG_MSG("Buddy info of SLAB Allocator:");
    slabAlloc->DumpBuddyInfo(CurrentHAL->GetCurrentDebugOutputDevice());
    
    MAIN_DEBUG_MSG("Existing SLABs in SLAB Allocator:");
    slabAlloc->DumpCacheInfo(CurrentHAL->GetCurrentDebugOutputDevice());
#endif
#endif

#if 0
    KernelModule* obj = GXAllocateFromType(KernelModule);
    obj->Plug();
    GXSafeReleaseNULL(obj);
    
    TestObject* object = GXAllocateFromType(TestObject);
    object->doSomething();
    object->Release();
#endif

    while(1);
}
