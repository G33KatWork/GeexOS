#include <kernel/global.h>
#include <arch/interrupts.h>

using namespace IO;
using namespace Arch;

Monitor kout = Monitor();

void panic(const char *message)
{
    DisableInterrupts();
    
    kout.SetBackground(Black);
    kout.SetForeground(Red);
    kout << "[PANIC] Kernel Panic: " << message << endl;
    
    HaltMachine();
}

void panic_assert(const char *file, unsigned int line, const char *desc)
{
    DisableInterrupts();
    
    kout.SetBackground(Black);
    kout.SetForeground(Red);
    kout << "[PANIC] Kernel Panic: Assertion failed at " << file << ":" << dec << line << " (" << desc << ")" << endl;
    
    HaltMachine();
}
