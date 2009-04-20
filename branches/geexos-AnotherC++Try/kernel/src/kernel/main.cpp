#include <arch/hal.h>
#include <kernel/IO/Monitor.h>
#include <kernel/global.h>
#include <kernel/multiboot.h>
#include <kernel/debug.h>
#include <kernel/Memory/Paging.h>
#include <kernel/Memory/PageFaultHandler.h>
#include <arch/interrupts.h>

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
    //Prepare monitor output
    kout.Clear();
    kout << "GeexOS loading..." << endl;
    
    //Initialize paging
    Paging *p = new Paging();
    p->Init();
    
    //Flush GDT and initialize IDT (Interrupts)
    InitializeCPU();
    
    //Get information about environment from GRUB
    Multiboot m = Multiboot(multibootInfo);
    
    //Init symtab and strtab for stacktraces
    Debug::stringTable = m.StrtabStart();
    Debug::symbolTable = m.SymtabStart();
    
    //Configure interrupt dispatcher
    InterruptDispatcher* irqD = InterruptDispatcher::GetInstance();
    irqD->RegisterHandler(14, new PageFaultHandler());
    
    InitializeTimer(100);
    Arch::EnableInterrupts();
    
    //Print some debugging stuff
    DEBUG_MSG("Kernel entrypoint reached. Commandline: " << m.GetKernelCommandline());
    DEBUG_MSG("I'm a DEBUG message!");
    
    //BÄM!
    int* bla = (int*)0xA0000000;
    *bla = 1;
    
    /*for(;;)
    {
        kout << Monitor::SetPosition(20, 20) << dec << (int)GetTickCount();
    }*/
    
    a();
    //PANIC("bla");
    //ASSERT(1 < 0);
    
    return 0; 
}
