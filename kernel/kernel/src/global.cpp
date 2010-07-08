#include <kernel/global.h>
#include <arch/hal.h>
#include <kernel/Memory/PlacementAllocator.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <kernel/debug.h>
#include <arch/hal.h>

using namespace IO;
using namespace Arch;
using namespace Memory;

PlacementAllocator placementAlloc = PlacementAllocator();
BaseDebugOutputDevice* kdbg = NULL;

void doPanic()
{
    CurrentHAL->DisableInterrupts();
    
    /*kdbg.SetForeground(Red);
    kdbg << "[PANIC] Kernel Panic: " << message << endl;*/
    
    if(VirtualMemoryManager::GetInstance()->KernelStack() != NULL)
        VirtualMemoryManager::GetInstance()->KernelStack()->PrintStacktrace();
    
    CurrentHAL->HaltMachine();
}

void panic_assert(const char *file, unsigned int line, const char *condition, const char *desc)
{
    CurrentHAL->DisableInterrupts();
    
    kdbg->SetForeground(Red);
    *kdbg << "[PANIC] Kernel Panic: Assertion failed at " << file << ":" << dec << line << " (" << condition << ") " << desc << endl;
    
    if(VirtualMemoryManager::GetInstance()->KernelStack() != NULL)
        VirtualMemoryManager::GetInstance()->KernelStack()->PrintStacktrace();
    
    CurrentHAL->HaltMachine();
}

void *operator new(size_t size)
{
    return placementAlloc.Allocate(size, false);
}

void *operator new[](size_t size)
{
    return placementAlloc.Allocate(size, false);
}

void  operator delete(void *p)
{
    
}

void  operator delete[](void *p)
{
    
}

void *operator new(size_t size, bool pageAllocation)
{
    return placementAlloc.Allocate(size, pageAllocation);
}

void operator delete(void *p, bool UNUSED(pageAllocation))
{
    
}

void* kmalloc(size_t size)
{
    return placementAlloc.Allocate(size, false);
}

void kfree(void* p)
{
    
}

unsigned int getPlacementPointer()
{
    return placementAlloc.GetPointerPosition();
}
