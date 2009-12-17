#include <arch/hal.h>
#include <kernel/IO/Monitor.h>
#include <kernel/global.h>
#include <kernel/multiboot.h>
#include <kernel/debug.h>
#include <arch/PageFaultHandler.h>
#include <kernel/Memory/PlacementAllocator.h>
#include <arch/interrupts.h>
#include <kernel/IInterruptServiceRoutine.h>
#include <kernel/Time/TimerManager.h>
#include <kernel/Time/Timer.h>
#include <kernel/Memory/Stack.h>
#include <kernel/Memory/VirtualMemoryManager.h>

extern      Address             bootStack;
#define     BOOTSTACK_SIZE      0x1000              //change this in start.S, too!

using namespace Arch;
using namespace Kernel;
using namespace IO;
using namespace Memory;
using namespace Processes;
using namespace Time;

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

class ExceptionHandler : public IInterruptServiceRoutine
{
public:
    void Execute(registers_t *regs)
    {
        DEBUG_MSG("Exception " << dec << regs->int_no << ": EIP: " << hex << (unsigned) regs->eip);
        PANIC("Unhandled exception!");
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
    
    void Execute(registers_t* UNUSED(regs))
    {
        this->tm->HandleTick(&Arch::ClockSource);
        /*if(this->tm->HandleTick(&Arch::ClockSource))
            Scheduler::GetInstance()->Schedule();*/
    }
};

bool timerExpired(void)
{
    DEBUG_MSG("The timer has expired");
    return false; //should the scheduler pick a new process?
}

class KeyboardHandler : public IInterruptServiceRoutine
{
private:
    TimerManager *tm;
    
public:
    KeyboardHandler(TimerManager* timerManager)
    {
        tm = timerManager;
    }
    
    void Execute(registers_t* UNUSED(regs))
    {
        Timer *t = new Timer(FUNCTION, timerExpired, NULL);
        tm->StartTimer(t, 1000000000);
        DEBUG_MSG("Timer started");
    }
};

int main(MultibootHeader* multibootInfo)
{   
    //Prepare monitor output
    kdbg.Clear();
    DEBUG_MSG("GeexOS Kernel booting...");
    
    //Get information about environment from GRUB
    DEBUG_MSG("Multiboot structure is at " << hex << (unsigned)multibootInfo);
    Multiboot m = Multiboot(multibootInfo);
    
    //Flush GDT and initialize IDT (Interrupts)
    InitializeCPU();
    DEBUG_MSG("CPU and Interrupt tables initialized...");
    
    //Initialize Memory
    VirtualMemoryManager *mm = new VirtualMemoryManager(m.GetLowerMemory() + m.GetUpperMemory());
    
    /*DEBUG_MSG("Setting up new stack at " << hex << KSTACK_LOCATION << " with size of " << dec << KSTACK_SIZE/1024 << " KB");
    Stack *stack = new Stack(KSTACK_LOCATION, KSTACK_SIZE);
    stack->AllocateSpace();
    stack->MoveCurrentStackHere((Address)&bootStack);
    memoryManager.SetKernelStack(stack);
    DEBUG_MSG("Stack seems to be successfully moved...");*/
    
    //Init symtab and strtab for stacktraces
    Debug::stringTable = m.elfInfo->GetSection(".strtab");
    Debug::symbolTable = m.elfInfo->GetSection(".symtab");
    
    Elf32SectionHeader *text = m.elfInfo->GetSection(".text");
    DEBUG_MSG("Text address: " << hex << text->addr);
    Elf32SectionHeader *data = m.elfInfo->GetSection(".data");
    DEBUG_MSG("Data address: " << hex << data->addr);
    Elf32SectionHeader *rodata = m.elfInfo->GetSection(".rodata");
    DEBUG_MSG("Rodata address: " << hex << rodata->addr);
    Elf32SectionHeader *bss = m.elfInfo->GetSection(".bss");
    DEBUG_MSG("BSS address: " << hex << bss->addr);
    
    DEBUG_MSG("Kernel commandline: " << m.GetKernelCommandline());
    
    //Configure interrupt dispatcher
    InterruptDispatcher* irqD = InterruptDispatcher::GetInstance();
    irqD->RegisterHandler(14, new PageFaultHandler());
    irqD->RegisterHandler(6, new InvalidOpcodeHandler());
    ExceptionHandler *ex = new ExceptionHandler();
    irqD->RegisterHandler(0, ex);
    irqD->RegisterHandler(1, ex);
    irqD->RegisterHandler(2, ex);
    irqD->RegisterHandler(3, ex);
    irqD->RegisterHandler(4, ex);
    irqD->RegisterHandler(5, ex);
    irqD->RegisterHandler(7, ex);
    irqD->RegisterHandler(8, ex);
    irqD->RegisterHandler(9, ex);
    irqD->RegisterHandler(10, ex);
    irqD->RegisterHandler(11, ex);
    irqD->RegisterHandler(12, ex);
    irqD->RegisterHandler(13, ex);
    irqD->RegisterHandler(15, ex);
    irqD->RegisterHandler(16, ex);
    DEBUG_MSG("Interrupt dispatcher initialized...");
    
    //Init timer
    InitializeTimer();
    TimerManager *tm = new TimerManager(&Arch::ClockSource);
    irqD->RegisterHandler(IRQ_TIMER, new TimerHandler(tm));
    DEBUG_MSG("Timer initialized...");
    
    Arch::EnableInterrupts();
    DEBUG_MSG("Interrupts enabled...");
    
    irqD->RegisterHandler(IRQ_KEYBOARD, new KeyboardHandler(tm));
    Arch::UnmaskIRQ(IRQ_KEYBOARD);
    
    DEBUG_MSG("Placement pointer is at " << hex << getPlacementPointer());
    
    for(;;) {
        //scheduler->Schedule();
        asm volatile("hlt"); //halt cpu until next irq (timer etc.) to switch to next time slice
    }
    
    return 0; 
}
