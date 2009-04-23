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

void *operator new(size_t size, bool pageAlign, Address *addr)
{
    return memoryManager.kmalloc(size, pageAlign, addr);
}

void *operator new[](size_t size)
{
    return memoryManager.kmalloc(size, false);
}

void *operator new[](size_t size, bool pageAlign, Address *addr)
{
    return memoryManager.kmalloc(size, pageAlign, addr);
}

void  operator delete(void *p)
{
    memoryManager.free(p);
}

void  operator delete[](void *p)
{
    memoryManager.free(p);
}
