#include <kernel/Processes/Scheduler.h>
#include <kernel/DataStructures/OrderedArray.h>
#include <arch/scheduling.h>
#include <arch/interrupts.h>


using namespace Processes;
using namespace Arch;

void idleTask() {
    for(;;)
    {
        asm volatile("int $3");
        kout << "a";
    }
}

void proc2() {
    for(;;)
    {
        asm volatile("int $4");
        kout << "b";
    }
}

Scheduler* Scheduler::instance = NULL;

Scheduler* Scheduler::GetInstance()
{
    if(instance == NULL)
        instance = new Scheduler();
    
    return instance;
}

static bool processLessThan(Process *p1, Process *p2)
{
    return p1->GetLastRun() < p2->GetLastRun();
}

Scheduler::Scheduler()
{
    processQueue = new OrderedArray<Process*, 128>();
    processQueue->SetPredicate(processLessThan);
    
    currentProcess = new Process(1, 0x400000, Paging::GetInstance()->GetKernelDirectory());
    currentProcess->SetInstructionPointer((Address)idleTask);
    
    Process* proc2 = new Process(2, 0x350000, Paging::GetInstance()->GetKernelDirectory());
    proc2->SetInstructionPointer((Address)proc2);
    processQueue->Insert(proc2);
    
    DEBUG_MSG("Scheduler initialized");
}

void Scheduler::Schedule()
{
    if (processQueue->GetSize())
    {
        Process *next = processQueue->ItemAt(0);
        processQueue->RemoveAt(0);
        
        //TODO: Check status if zombie, blocked etc. and schedule only runnable tasks
        processQueue->Insert(currentProcess);
        
        next->UpdateLastRun();
        
        switchToProcess(next);
    }
}

void Scheduler::switchToProcess(Process* p)
{
    kout << "Switching to process " << dec << p->GetPid() << endl;
    
    /*DisableInterrupts();
    
    currentProcess->SetStackPointer(readStackPointer());
    currentProcess->SetBasePointer(readBasePointer());
    //save old instruction pointer
    currentProcess->SetInstructionPointer(interruptRegs->eip);
    
    
    //******* SWITCH TO NEW PROC ********
    currentProcess = p;
    
    interruptRegs->esp = currentProcess->GetStackPointer();
    interruptRegs->ebp = currentProcess->GetBasePointer();    
    Paging::GetInstance()->SwitchCurrentPageDirectory(p->GetPageDirectory());
    interruptRegs->eip = currentProcess->GetInstructionPointer();*/ //set new eip in struct so the iret of the interrupt handler jumps to other process

    //TODO: TSS?
}

void Scheduler::AddProcess(Process* p)
{
    processQueue->Insert(p);
}

