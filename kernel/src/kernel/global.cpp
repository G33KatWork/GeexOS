#include <kernel/global.h>
#include <arch/hal.h>
#include <arch/interrupts.h>
#include <kernel/debug.h>
#include <kernel/Memory/MemoryManager.h>

using namespace IO;
using namespace Arch;
using namespace Debug;
using namespace Memory;

Monitor kout = Monitor();
MemoryManager memoryManager = MemoryManager();

void panic(const char *message)
{
    DisableInterrupts();
    
    kout.SetForeground(Red);
    kout << "[PANIC] Kernel Panic: " << message << endl;
    
    PrintStacktrace();
    
    HaltMachine();
}

void panic_assert(const char *file, unsigned int line, const char *condition, const char *desc)
{
    DisableInterrupts();
    
    kout.SetForeground(Red);
    kout << "[PANIC] Kernel Panic: Assertion failed at " << file << ":" << dec << line << " (" << condition << ") " << desc << endl;
    
    PrintStacktrace();
    
    HaltMachine();
}

void *operator new(size_t size)
{
    return memoryManager.kmalloc(size, false);
}

void *operator new[](size_t size)
{
    return memoryManager.kmalloc(size, false);
}

void  operator delete(void *p)
{
    memoryManager.kfree(p);
}

void  operator delete[](void *p)
{
    memoryManager.kfree(p);
}

void *operator new(size_t size, bool pageAllocation)
{
    return memoryManager.kmalloc(size, pageAllocation);
}

void operator delete(void *p, bool pageAllocation)
{
    //FIXME: Implement deallocation mechanism for PageTables and PageDirectories
}

void* kmalloc(size_t size)
{
    return memoryManager.kmalloc(size, false);
}

void kfree(void* p)
{
    memoryManager.kfree(p);
}
