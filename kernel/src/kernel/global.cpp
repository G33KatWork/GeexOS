#include <kernel/global.h>
#include <arch/hal.h>
#include <arch/interrupts.h>
#include <kernel/utils/DebuggingSymbols.h>
#include <kernel/Memory/PlacementAllocator.h>
#include <kernel/IO/Monitor.h>
#include <kernel/IO/SerialConsole.h>

using namespace IO;
using namespace Arch;
using namespace Debug;
using namespace Memory;

PlacementAllocator placementAlloc = PlacementAllocator();

#ifdef SERIAL_DEBUG
    SerialConsole kdbg = SerialConsole(SERIAL_COM1);
#else
    Monitor kdbg = Monitor();
#endif

void panic(const char *message)
{
    DisableInterrupts();
    
    kdbg.SetForeground(Red);
    kdbg << "[PANIC] Kernel Panic: " << message << endl;
    
    //memoryManager.GetKernelStack()->PrintStacktrace();
    
    HaltMachine();
}

void panic_assert(const char *file, unsigned int line, const char *condition, const char *desc)
{
    DisableInterrupts();
    
    kdbg.SetForeground(Red);
    kdbg << "[PANIC] Kernel Panic: Assertion failed at " << file << ":" << dec << line << " (" << condition << ") " << desc << endl;
    
    //memoryManager.GetKernelStack()->PrintStacktrace();
    
    HaltMachine();
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
