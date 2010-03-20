#include <arch/hal.h>
#include <kernel/IO/Monitor.h>
#include <kernel/global.h>
#include <kernel/multiboot.h>
#include <kernel/utils/DebuggingSymbols.h>
#include <arch/PageFaultHandler.h>
#include <kernel/Memory/PlacementAllocator.h>
#include <arch/interrupts.h>
#include <kernel/IInterruptServiceRoutine.h>
#include <kernel/Time/TimerManager.h>
#include <kernel/Time/Timer.h>
#include <kernel/Memory/Stack.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <kernel/Memory/Virtual/VirtualMemorySpace.h>

extern      Address             bootStack;
//#define     BOOTSTACK_SIZE      0x1000              //change this in start.S, too!
#define     STACK_ADDRESS       0xFFFF0000          //Uppermost address -64 KByte
#define     STACK_SIZE          0x10000             //64KByte

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
        MAIN_DEBUG_MSG("Invalid Opcode: EIP: " << hex << (unsigned) regs->eip);
        PANIC("Invalid opcode!");
        for(;;);
    }
};

class ExceptionHandler : public IInterruptServiceRoutine
{
public:
    void Execute(registers_t *regs)
    {
        MAIN_DEBUG_MSG("Exception " << dec << regs->int_no << ": EIP: " << hex << (unsigned) regs->eip);
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
    MAIN_DEBUG_MSG("The timer has expired");
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
        MAIN_DEBUG_MSG("Timer started");
    }
};

int main(MultibootHeader* multibootInfo)
{   
    //Prepare monitor output
    kdbg.Clear();
    MAIN_DEBUG_MSG("GeexOS Kernel booting...");
    
    //Get information about environment from GRUB
    MAIN_DEBUG_MSG("Multiboot structure is at " << hex << (unsigned)multibootInfo);
    Multiboot m = Multiboot(multibootInfo);
    
    //Flush GDT and initialize IDT (Interrupts)
    InitializeCPU();
    MAIN_DEBUG_MSG("CPU and Interrupt tables initialized...");
    
    //Initialize Memory
    VirtualMemoryManager *mm = new VirtualMemoryManager(m.GetLowerMemory() + m.GetUpperMemory());
    
    //Init symtab and strtab for stacktraces
    Debug::stringTable = m.elfInfo->GetSection(".strtab");
    Debug::symbolTable = m.elfInfo->GetSection(".symtab");
    
    //Build virtual memory space
    VirtualMemorySpace *vm = new VirtualMemorySpace();
    
    Elf32SectionHeader *text = m.elfInfo->GetSection(".text");
    VirtualMemoryRegion* textRegion = new VirtualMemoryRegion(((Address)text->addr) & IDENTITY_POSITION, (size_t)text->size, ".text", ALLOCFLAG_EXECUTABLE);
    textRegion->SetFlags(textRegion->GetFlags());
    vm->AddRegion(textRegion);
    
    Elf32SectionHeader *data = m.elfInfo->GetSection(".data");
    VirtualMemoryRegion* dataRegion = new VirtualMemoryRegion((Address)data->addr, (size_t)data->size, ".data", ALLOCFLAG_WRITABLE);
    dataRegion->SetFlags(dataRegion->GetFlags());
    vm->AddRegion(dataRegion);
    
    Elf32SectionHeader *rodata = m.elfInfo->GetSection(".rodata");
    VirtualMemoryRegion* rodataRegion = new VirtualMemoryRegion((Address)rodata->addr, (size_t)rodata->size, ".rodata", ALLOCFLAG_NONE);
    rodataRegion->SetFlags(rodataRegion->GetFlags());
    vm->AddRegion(rodataRegion);
    
    Elf32SectionHeader *bss = m.elfInfo->GetSection(".bss");
    VirtualMemoryRegion* bssRegion = new VirtualMemoryRegion((Address)bss->addr, (size_t)bss->size, ".bss", ALLOCFLAG_WRITABLE);
    bssRegion->SetFlags(bssRegion->GetFlags());
    vm->AddRegion(bssRegion);
    
    //VirtualMemoryRegion* stackRegion = new VirtualMemoryRegion(STACK_ADDRESS, STACK_SIZE, "Kernel Stack");
    
    //vm->DumpRegions(kdbg);
    
    /*MAIN_DEBUG_MSG("Setting up new stack at " << hex << KSTACK_LOCATION << " with size of " << dec << KSTACK_SIZE/1024 << " KB");
    Stack *stack = new Stack(KSTACK_LOCATION, KSTACK_SIZE);
    stack->AllocateSpace();
    stack->MoveCurrentStackHere((Address)&bootStack);
    memoryManager.SetKernelStack(stack);
    MAIN_DEBUG_MSG("Stack seems to be successfully moved...");*/
    
    MAIN_DEBUG_MSG("Kernel commandline: " << m.GetKernelCommandline());
    
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
    MAIN_DEBUG_MSG("Interrupt dispatcher initialized...");
    
    //Init timer
    InitializeTimer();
    TimerManager *tm = new TimerManager(&Arch::ClockSource);
    irqD->RegisterHandler(IRQ_TIMER, new TimerHandler(tm));
    MAIN_DEBUG_MSG("Timer initialized...");
    
    Arch::EnableInterrupts();
    MAIN_DEBUG_MSG("Interrupts enabled...");
    
    //int* a = (int*)0x100000;
    //*a = 0x41414141;
    
    irqD->RegisterHandler(IRQ_KEYBOARD, new KeyboardHandler(tm));
    Arch::UnmaskIRQ(IRQ_KEYBOARD);
    
    MAIN_DEBUG_MSG("Placement pointer is at " << hex << getPlacementPointer());
    
    for(;;) {
        //scheduler->Schedule();
        asm volatile("hlt"); //halt cpu until next irq (timer etc.) to switch to next time slice
    }
    
    return 0; 
}
