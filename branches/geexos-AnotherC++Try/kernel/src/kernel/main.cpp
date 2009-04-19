#include <arch/hal.h>
#include <kernel/IO/Monitor.h>
#include <kernel/global.h>
#include <kernel/multiboot.h>
#include <kernel/debug.h>
#include <kernel/Memory/Paging.h>

using namespace Arch;
using namespace Kernel;
using namespace IO;
using namespace Memory;

void d()
{
    asm volatile("nop");
    PANIC("test");
}

void c()
{
    asm volatile("nop");
    d();
}

void b()
{
    asm volatile("nop");
    c();
}

void a()
{
    asm volatile("nop");
    b();
}

int main(MultibootHeader* multibootInfo)
{
    kout.Clear();
    kout << "GeexOS loading..." << endl;
    
    Paging *p = new Paging();
    p->Init();
    
    InitializeCPU();
    
    Multiboot m = Multiboot(multibootInfo);
    
    //Init symtab and strtab for stacktraces
    Debug::stringTable = m.StrtabStart();
    Debug::symbolTable = m.SymtabStart();
    
    DEBUG_MSG("Kernel entrypoint reached. Commandline: " << m.GetKernelCommandline());
    DEBUG_MSG("I'm a DEBUG message!");
    
    a();
    //PANIC("bla");
    //ASSERT(1 < 0);
        
    return 0; 
}
