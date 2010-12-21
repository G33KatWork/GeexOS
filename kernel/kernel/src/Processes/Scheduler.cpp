#include <kernel/Processes/Scheduler.h>
#include <kernel/Time/TimerManager.h>
#include <arch/HAL.h>
#include <kernel/debug.h>

using namespace Processes;
using namespace Arch;
using namespace Time;
using namespace Debug;

/// Length of timeslice a thread gets until reschedulung occurs in microeconds
#define     SCHED_THREAD_TIMESLICE      10000

bool Scheduler::TimerExpired(Timer* t)
{
    //Readd timer
    tm->StartTimer(t, SCHED_THREAD_TIMESLICE);
    
    //Update timeslice of current process
    Thread* curThread = this->GetCurrentThread();
    curThread->SetTimeslice(curThread->GetTimeslice() - CurrentHAL->GetHardwareClockSource()->GetTickLengthUs());
    
    SCHEDULER_DEBUG_MSG("Timeslice of current thread: " << dec << curThread->GetTimeslice());
    
    if(curThread->GetTimeslice() <= 0)
        return true; //force scheduling
    else
        return false;
}

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
    
    schedulingTimer = new Timer(OOPCALLBACK, NULL, NULL, this);
    
    tm = NULL;
    nextId = 1;
    
    SCHEDULER_DEBUG_MSG("Scheduler initialized");
    
    CurrentHAL->EnableInterrupts();
}

void Scheduler::SetTimerManager(TimerManager* t)
{
    tm = t;
    currentThread->SetTimeslice(SCHED_THREAD_TIMESLICE);
    tm->StartTimer(schedulingTimer, SCHED_THREAD_TIMESLICE);
}

void Scheduler::AddThread(Thread* thread)
{
    thread->next = listHead;
    listHead = thread;
}

void Scheduler::Schedule(registers_t* oldState)
{
    if(tm == NULL) return;
    //check if we have a timer manager
    ASSERT(tm != NULL, "TimerManager is NULL");
    
    SCHEDULER_DEBUG_MSG("Entering scheduler");
    CurrentHAL->DisableInterrupts();
    
    currentThread->SaveThreadContext(oldState);
    
    Thread *next;
    if(currentThread->next == NULL)
        next = listHead;
    else
        next = currentThread->next;

	SCHEDULER_DEBUG_MSG("Picking thread " << next->GetName());
	currentThread = next;
    
    currentThread->SetTimeslice(SCHED_THREAD_TIMESLICE);
    
    //printThreadInfo(currentThread->threadInfo);
    
    currentThread->SwitchTo();
}

void Scheduler::DumpThreads(BaseDebugOutputDevice* c)
{
    for(Thread* curThread = listHead; curThread != NULL; curThread = curThread->next)
    {
        *c << "SCHEDULER: " << "\tThread ID: " << dec << curThread->GetId() << endl;
        *c << "SCHEDULER: " << "\tThread Name: " << curThread->GetName() << endl;
        *c << "SCHEDULER: " << "\tInstruction Pointer: " << hex << curThread->GetInstructionPointer() << endl;
        *c << "SCHEDULER: " << "\tStack Pointer: " << curThread->GetStackPointer() << endl;
        *c << "SCHEDULER: " << "\tBase Pointer: " << curThread->GetFramePointer() << endl;
        *c << "SCHEDULER: " << "\tTimeslice: " << dec << curThread->GetTimeslice() << endl;
        
        *c << endl;
    }
}
