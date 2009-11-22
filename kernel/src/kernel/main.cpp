#include <arch/hal.h>
#include <kernel/IO/Monitor.h>
#include <kernel/global.h>
#include <kernel/multiboot.h>
#include <kernel/debug.h>
#include <arch/Paging.h>
#include <arch/PageFaultHandler.h>
#include <kernel/Memory/PlacementAllocator.h>
#include <kernel/Memory/BitfieldPhysicalMemoryManager.h>
#include <kernel/Memory/Heap.h>
#include <arch/interrupts.h>
#include <kernel/Processes/Scheduler.h>
#include <kernel/Processes/Thread.h>
#include <kernel/IInterruptServiceRoutine.h>
#include <kernel/Time/TimerManager.h>
#include <kernel/Time/Timer.h>
#include <kernel/Memory/Stack.h>

//see doc/memory_layout.txt
#define     KHEAP_LOCATION      0xC0400000
#define     KHEAP_MAX_SIZE      512 * 1024 * 1024
#define     KHEAP_INITIAL_SIZE  1 * 1024

#define     KSTACK_LOCATION     0xE0400000
#define     KSTACK_SIZE         16 * 1024

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
        if(this->tm->HandleTick(&Arch::ClockSource))
            Scheduler::GetInstance()->Schedule();
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

void thread(void* arg)
{
    kdbg << "Thread 1 start... Arg: " << (char *)arg << endl;
    
    for(;;)
    {
        kdbg << "b";
    }
}

void thread2(void* arg)
{
    kdbg << "Thread 2 start... Arg: " << (char *)arg << endl;
    
    for(;;)
    {
        kdbg << "a";
    }
}

int main(MultibootHeader* multibootInfo)
{
    PlacementAllocator placementAllocator = PlacementAllocator();
    memoryManager.SetAllocator(&placementAllocator);
    
    //Prepare monitor output
    kdbg.Clear();
    
    //Setup temporary bootstack - will be moved later when we have full blown paging
    DEBUG_MSG("Initial boot stackpointer is: " << hex << (unsigned)&bootStack);
    Stack bStack = Stack((Address)&bootStack, BOOTSTACK_SIZE);
    memoryManager.SetKernelStack(&bStack);
    
    //Initialize paging
    Paging::GetInstance()->Init();
    DEBUG_MSG("Paging initialized...");
    
    //Flush GDT and initialize IDT (Interrupts)
    InitializeCPU();
    DEBUG_MSG("CPU and Interrupts initialized...");
    
    //Get information about environment from GRUB
    DEBUG_MSG("Multiboot structure is at " << hex << (unsigned)multibootInfo);
    Multiboot m = Multiboot(multibootInfo);
    
    //Give our memory manager some information about the installed RAM
    //The allocator is needed by the Paging class to get space for its page
    //directory and page table objects
    //TODO: use memory map
    DEBUG_MSG("Installed amount of memory seems to be " << dec << m.GetLowerMemory() + m.GetUpperMemory() << "KB");
    BitfieldPhysicalMemoryManager *pm = new BitfieldPhysicalMemoryManager(m.GetLowerMemory() + m.GetUpperMemory());
    memoryManager.SetPhysicalMemoryManager(pm);
    DEBUG_MSG("Frame allocator initialized...");
    
    DEBUG_MSG("Setting up new stack at " << hex << KSTACK_LOCATION << " with size of " << dec << KSTACK_SIZE/1024 << " KB");
    Stack *stack = new Stack(KSTACK_LOCATION, KSTACK_SIZE);
    stack->AllocateSpace();
    stack->MoveCurrentStackHere((Address)&bootStack);
    memoryManager.SetKernelStack(stack);
    DEBUG_MSG("Stack seems to be successfully moved...");
    
    //Init symtab and strtab for stacktraces
    Debug::stringTable = m.StrtabStart();
    Debug::symbolTable = m.SymtabStart();
    
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

    DEBUG_MSG("Setting up heap, starting at " << hex << KHEAP_LOCATION << " with maximum size of " << dec << KHEAP_MAX_SIZE/1024 << "KB and an initial size of " << KHEAP_INITIAL_SIZE/1024 << "KB");
    Heap *h = new Heap(KHEAP_LOCATION, KHEAP_MAX_SIZE, KHEAP_INITIAL_SIZE);
    
    int *a = (int*)h->Allocate(sizeof(int), false);
    int *b = (int*)h->Allocate(sizeof(int), false);
    DEBUG_MSG("Allocated 2 integer on heap: A: " << hex << (unsigned)a << " B: " << (unsigned)b);

    int *c = (int*)h->Allocate(sizeof(int), true);
    DEBUG_MSG("Allocated page aligned integer on heap: " << hex << (unsigned)c);
    
    void *d = h->Allocate(1024*1024, false);
    DEBUG_MSG("Allocated 1MB not page aligned on heap: " << hex << (unsigned)d);
    
    void *e = h->Allocate(2*1024*1024, false);
    DEBUG_MSG("Allocated 2MB not page aligned on heap: " << hex << (unsigned)e);
    
    DEBUG_MSG("Dumping current heap state to COM1...");
    SerialConsole ser = SerialConsole(SERIAL_COM1);
    h->DumpCurrentStructure(ser);
    
    //Init timer
    InitializeTimer();
    TimerManager *tm = new TimerManager(&Arch::ClockSource);
    irqD->RegisterHandler(IRQ_TIMER, new TimerHandler(tm));
    DEBUG_MSG("Timer initialized...");
    
    Arch::EnableInterrupts();
    DEBUG_MSG("Interrupts enabled...");
    
    irqD->RegisterHandler(IRQ_KEYBOARD, new KeyboardHandler(tm));
    Arch::UnmaskIRQ(IRQ_KEYBOARD);
    
    /*Scheduler* scheduler = Scheduler::GetInstance();
    scheduler->SetTimerManager(tm);
    DEBUG_MSG("Scheduler initialized");*/
    
    //Thread *t = new Thread(thread, NULL, 10, NULL);
    //scheduler->AddThread(t);
    
    //Thread *t2 = new Thread(thread2, NULL, 10, NULL);
    //scheduler->AddThread(t2);
    
    for(;;) {
        //scheduler->Schedule();
        asm volatile("hlt"); //halt cpu until next irq (timer etc.) to switch to next time slice
    }
    
    return 0; 
}
