#include <kernel/global.h>
#include <arch/hal.h>
#include <kernel/Memory/PlacementAllocator.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <kernel/debug.h>
#include <arch/hal.h>
#include <string.h>

using namespace Debug;
using namespace Arch;
using namespace Memory;

PlacementAllocator placementAlloc = PlacementAllocator();

void doPanic()
{
    CurrentHAL->DisableInterrupts();
    
    /*kdbg.SetForeground(Red);
    kdbg << "[PANIC] Kernel Panic: " << message << endl;*/
    
    /*if(VirtualMemoryManager::GetInstance()->KernelStack() != NULL)
        VirtualMemoryManager::GetInstance()->KernelStack()->PrintStacktrace();*/
    
    CurrentHAL->HaltMachine();
}

void panic_assert(const char *file, unsigned int line, const char *condition, const char *desc)
{
    CurrentHAL->DisableInterrupts();
    
    CurrentHAL->GetCurrentDebugOutputDevice()->SetForeground(Red);
    *CurrentHAL->GetCurrentDebugOutputDevice() << "[PANIC] Kernel Panic: Assertion failed at " << file << ":" << dec << line << " (" << condition << ") " << desc << endl;
    
    /*if(VirtualMemoryManager::GetInstance()->KernelStack() != NULL)
        VirtualMemoryManager::GetInstance()->KernelStack()->PrintStacktrace();*/
    
    CurrentHAL->HaltMachine();
}

void *operator new(size_t size) { return kmalloc(size); }
void *operator new[](size_t size) { return kmalloc(size); }

void operator delete(void *p) { kfree(p); }
void operator delete[](void *p) { kfree(p); }

void *operator new(size_t UNUSED(size), void* buffer) { return buffer; }
void *operator new[](size_t UNUSED(size), void* buffer) { return buffer; }

void *operator new(size_t size, bool pageAllocation)
{
    return placementAlloc.Allocate(size, pageAllocation);
}

void operator delete(void *p, bool UNUSED(pageAllocation))
{
    
}

void* kmalloc(size_t size)
{
    if(VirtualMemoryManager::GetInstance()->SlabAllocator() != NULL)
        return Slab::AllocateFromSizeSlabs(size);
    else    
        return placementAlloc.Allocate(size, false);
}

void kfree(void* p)
{
    Address addr = (Address)p;
    
    if(addr >= VirtualMemoryManager::GetInstance()->SlabAllocator()->StartAddress() &&
       addr < VirtualMemoryManager::GetInstance()->SlabAllocator()->StartAddress() + VirtualMemoryManager::GetInstance()->SlabAllocator()->Size())
        Slab::FreeFromSizeSlabs(p);
    else
        PANIC("It seems that you are trying to free an object at " << hex << addr << " not residing in the slab allocator. Not good!");
}

/*void* krealloc(void* p, size_t s)
{
    if(s <= 0)
    {
        if(p) kfree(p);
        return NULL;
    }
    else if(!p)
        return kmalloc(s);
    else
    {
        void* newBuf = kmalloc(s);
        if(newBuf)
        {
            memcpy(newBuf, p, s);       //FIXME: Broken! Copies too much for some cases and could lead to a pagefault :-/
            kfree(p);
        }
        
        return newBuf;
    }
}*/

unsigned int getPlacementPointer()
{
    return placementAlloc.GetPointerPosition();
}
