#include <arch/hal.h>
#include <kernel/IO/Monitor.h>
#include <kernel/global.h>
#include <kernel/multiboot.h>
#include <kernel/debug.h>
#include <kernel/Memory/Paging.h>
#include <kernel/Memory/PageFaultHandler.h>
#include <kernel/Memory/PlacementAllocator.h>
#include <kernel/Memory/BitfieldPhysicalMemoryManager.h>
#include <kernel/Memory/Heap.h>
#include <arch/interrupts.h>
#include <kernel/Processes/Scheduler.h>
#include <kernel/Processes/Thread.h>
#include <kernel/IInterruptServiceRoutine.h>
#include <kernel/Time/TimerManager.h>
#include <kernel/Time/Timer.h>

using namespace Arch;
using namespace Kernel;
using namespace IO;
using namespace Memory;
using namespace Processes;
using namespace Time;

extern Address bootStack;

class InvalidOpcodeHandler : public IInterruptServiceRoutine
{
public:
    void Execute(registers_t *regs)
    {
        DEBUG_MSG("Invalid Opcode: EIP: " << hex << (unsigned) regs->eip);
        PANIC("Invalid opcode!");
        for(;;);
    }
};

class TimerHandler : public IInterruptServiceRoutine
{
private:
    TimerManager *tm;
    
public:
    TimerHandler(TimerManager* timerManager)
    {
        tm = timerManager;
    }
    
    void Execute(registers_t *regs)
    {
        if(this->tm->HandleTick(&Arch::ClockSource))
            Scheduler::GetInstance()->Schedule();
            
        /*kout << Monitor::SetPosition(0, 15);
        kout << "EIP: " << hex << regs->eip << endl;
        kout << "CS: " << regs->cs << endl;
        kout << "EFLAGS: " << regs->eflags << endl;
        kout << "USERSP: " << regs->useresp << endl;
        kout << "SS: " << regs->ss << endl;*/
    }
};

void thread(void* arg)
{
    for(;;)
    {
        kout << "b";
    }
}

void thread2(void* arg)
{
    for(;;)
    {
        kout << "a";
    }
}

int main(MultibootHeader* multibootInfo)
{
    PlacementAllocator placementAllocator = PlacementAllocator();
    memoryManager.SetAllocator(&placementAllocator);
    
    //Prepare monitor output
    kout.Clear();
    kout << "GeexOS loading..." << endl;
    DEBUG_MSG("Boot stack starts at: " << hex << (unsigned)&bootStack);
    
    //Initialize paging
    Paging::GetInstance()->Init();
    DEBUG_MSG("Paging initialized...");
    
    //Flush GDT and initialize IDT (Interrupts)
    InitializeCPU();
    DEBUG_MSG("CPU and Interrupts initialized...");
    
    //Get information about environment from GRUB
    Multiboot m = Multiboot(multibootInfo);
    
    //Give our memory manager some information about the installed RAM
    //The allocator is needed by the Paging class to get space for its page
    //directory and page table objects
    //TODO: use memory map
    BitfieldPhysicalMemoryManager *pm = new BitfieldPhysicalMemoryManager(m.GetLowerMemory() + m.GetUpperMemory());
    memoryManager.SetPhysicalMemoryManager(pm);
    DEBUG_MSG("Frame allocator initialized...");
    
    //Init symtab and strtab for stacktraces
    Debug::stringTable = m.StrtabStart();
    Debug::symbolTable = m.SymtabStart();
    
    DEBUG_MSG("Kernel commandline: " << m.GetKernelCommandline());
    
    Heap *h = new Heap(0xC0400000, 512*1024*1024, 1*1024);
    memoryManager.SetAllocator(h);
    DEBUG_MSG("Kernel heap initialized...");
    DEBUG_MSG("Not really... Implement it, stupid sucker!")
    
    //Configure interrupt dispatcher
    InterruptDispatcher* irqD = InterruptDispatcher::GetInstance();
    irqD->RegisterHandler(14, new PageFaultHandler());
    irqD->RegisterHandler(6, new InvalidOpcodeHandler());
    DEBUG_MSG("Interrupt dispatcher initialized...");
    
    //Init timer
    InitializeTimer();
    TimerManager *tm = new TimerManager(&Arch::ClockSource);
    irqD->RegisterHandler(32, new TimerHandler(tm));
    DEBUG_MSG("Timer initialized...");
    
    Scheduler* scheduler = Scheduler::GetInstance();
    scheduler->SetTimerManager(tm);
    DEBUG_MSG("Scheduler initialized");
    
    //Thread *t = new Thread(thread, NULL, 10, NULL);
    //scheduler->AddThread(t);
    
    //Thread *t2 = new Thread(thread2, NULL, 10, NULL);
    //scheduler->AddThread(t2);
    
    Arch::EnableInterrupts();
    DEBUG_MSG("Interrupts enabled...");
    
    for(;;) {
        //scheduler->Schedule();
        asm volatile("hlt"); //halt cpu until next irq (timer etc.) to switch to next time slice
    }
    
    return 0; 
}





//Test some frame mapping
/*Paging::GetInstance()->MapAddress(0x30000000, 0x0, true, false);
int* bla = (int*)0x30000000;
DEBUG_MSG("Value at 0x30000000: " << hex << *bla);

//Allocate some frames
Address f1 = memoryManager.AllocateFrame();
Address f2 = memoryManager.AllocateFrame();
DEBUG_MSG("f1: " << hex << f1);
DEBUG_MSG("f2: " << f2);

//Print some debugging stuff
DEBUG_MSG("I'm a DEBUG message!");*

//BÄM!
/*int* bla2 = (int*)0xA0000000;
*bla2 = 1;*/

