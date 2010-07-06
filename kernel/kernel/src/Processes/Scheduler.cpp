#include <kernel/Processes/Scheduler.h>
#include <kernel/Time/TimerManager.h>
#include <arch/HAL.h>
#include <kernel/debug.h>

using namespace Processes;
using namespace Arch;
using namespace Time;
using namespace IO;

#define     SCHED_TIMER_FREQUENCY       10000000
#define     SCHED_THREAD_TIMESLICE      10000000

//FIXME: Make timer timers work again by fixing the List in TimerManager not being a memory eating monster

/*static bool scheduleTimerHandler(void)
{
    Thread* curThread = Scheduler::GetInstance()->GetCurrentThread();
    curThread->SetTimeslice(curThread->GetTimeslice() - SCHED_TIMER_FREQUENCY);
    
    SCHEDULER_DEBUG_MSG("Timeslice of current thread: " << dec << curThread->GetTimeslice());
    
    if(curThread->GetTimeslice() <= 0)
        return true; //force scheduling
    else
        return false;
}*/

Scheduler* Scheduler::instance = NULL;

Scheduler* Scheduler::GetInstance()
{
    if(instance == NULL)
        instance = new Scheduler();
    
    return instance;
}

Scheduler::Scheduler()
{
    CurrentHAL->DisableInterrupts();
    
    kernelThread = new Thread(0, 0, 0, 0, "Kernel thread", false, CurrentHAL->GetPaging()->GetKernelDirectory());
    kernelThread->next = NULL;
    listHead = kernelThread;
    currentThread = kernelThread;
    
    //schedulingTimer = new Timer(FUNCTION, scheduleTimerHandler, NULL);
    
    tm = NULL;
    nextId = 1;
    
    SCHEDULER_DEBUG_MSG("Scheduler initialized");
    
    CurrentHAL->EnableInterrupts();
}

void Scheduler::SetTimerManager(TimerManager* t)
{
    tm = t;
    //currentThread->SetTimeslice(SCHED_THREAD_TIMESLICE);
    //tm->StartTimer(schedulingTimer, SCHED_TIMER_FREQUENCY);
}

void Scheduler::AddThread(Thread* thread)
{
    thread->next = listHead;
    listHead = thread;
}

void Scheduler::Schedule(registers_t* oldState)
{
    SCHEDULER_DEBUG_MSG("Entering scheduler");
    CurrentHAL->DisableInterrupts();
    
    currentThread->SaveThreadContext(oldState);
    
    if(tm == NULL) return;
    //check if we have a timer manager
    ASSERT(tm != NULL, "TimerManager is NULL");
    
    Thread *next;
    if(currentThread->next == NULL)
        next = listHead;
    else
        next = currentThread->next;

	SCHEDULER_DEBUG_MSG("Picking thread " << next->GetName());
	currentThread = next;
    
    //currentThread->SetTimeslice(SCHED_THREAD_TIMESLICE);
    //tm->StartTimer(schedulingTimer, SCHED_TIMER_FREQUENCY);
    
    //printThreadInfo(currentThread->threadInfo);
    
    currentThread->SwitchTo();
}

void Scheduler::DumpThreads(CharacterOutputDevice& c)
{
    for(Thread* curThread = listHead; curThread != NULL; curThread = curThread->next)
    {
        c << "SCHEDULER: " << "\tThread ID: " << dec << curThread->GetId() << endl;
        c << "SCHEDULER: " << "\tThread Name: " << curThread->GetName() << endl;
        /*c << "SCHEDULER: " << "\tInstruction Pointer: " << hex << curThread->GetInstructionPointer() << endl;
        c << "SCHEDULER: " << "\tStack Pointer: " << curThread->GetStackPointer() << endl;
        c << "SCHEDULER: " << "\tBase Pointer: " << curThread->GetBasePointer() << endl;*/
        c << "SCHEDULER: " << "\tTimeslice: " << dec << curThread->GetTimeslice() << endl;
        
        c << endl;
    }
}
