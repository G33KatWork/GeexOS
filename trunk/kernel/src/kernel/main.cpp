#include <arch/hal.h>
#include <kernel/IO/Monitor.h>
#include <kernel/global.h>
#include <kernel/multiboot.h>
#include <kernel/debug.h>
#include <kernel/Memory/Paging.h>
#include <kernel/Memory/PageFaultHandler.h>
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
            
        kout << Monitor::SetPosition(0, 15);
        kout << "EIP: " << hex << regs->eip << endl;
        kout << "CS: " << regs->cs << endl;
        kout << "EFLAGS: " << regs->eflags << endl;
        kout << "USERSP: " << regs->useresp << endl;
        kout << "SS: " << regs->ss << endl;
    }
};

void thread(void* arg)
{
    for(;;)
    {
        //kout << "b";
    }
}

int main(MultibootHeader* multibootInfo)
{
    //Prepare monitor output
    kout.Clear();
    kout << "GeexOS loading..." << endl;
    kout << "Boot stack at: " << hex << (unsigned)&bootStack << endl;
    
    //Initialize paging
    Paging::GetInstance()->Init();
    kout << "Paging initialized..." << endl;
    
    //Flush GDT and initialize IDT (Interrupts)
    InitializeCPU();
    kout << "CPU and Interrupts initialized..." << endl;
    
    //Get information about environment from GRUB
    Multiboot m = Multiboot(multibootInfo);
    
    //Init symtab and strtab for stacktraces
    Debug::stringTable = m.StrtabStart();
    Debug::symbolTable = m.SymtabStart();
    
    DEBUG_MSG("Kernel commandline: " << m.GetKernelCommandline());
    
    //Give our memory manager some information about the installed RAM
    //TODO: use memory map
    memoryManager.InitializeFrameAllocator(m.GetLowerMemory() + m.GetUpperMemory());
    kout << "Frame allocator initialized..." << endl;
    
    //Configure interrupt dispatcher
    InterruptDispatcher* irqD = InterruptDispatcher::GetInstance();
    irqD->RegisterHandler(14, new PageFaultHandler());
    irqD->RegisterHandler(6, new InvalidOpcodeHandler());
    kout << "Interrupt dispatcher initialized..." << endl;
    
    //Init timer
    InitializeTimer();
    TimerManager *tm = new TimerManager(&Arch::ClockSource);
    irqD->RegisterHandler(32, new TimerHandler(tm));
    kout << "Timer initialized..." << endl;
    
    Scheduler* scheduler = Scheduler::GetInstance();
    scheduler->SetTimerManager(tm);
    kout << "Scheduler initialized" << endl;
    
    Thread *t = new Thread(thread, NULL, 10, NULL);
    scheduler->AddThread(t);
    
    Arch::EnableInterrupts();
    kout << "Interrupts enabled..." << endl;
    
    for(;;) {
        scheduler->Schedule();
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

