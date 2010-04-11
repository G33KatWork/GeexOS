#include <kernel/Processes/Scheduler.h>
#include <kernel/Time/TimerManager.h>
#include <arch/scheduling.h>
#include <kernel/debug.h>

using namespace Processes;
using namespace Arch;
using namespace Time;
using namespace IO;

#define     SCHED_TIMER_FREQUENCY       10000000
#define     SCHED_THREAD_TIMESLICE      10000000

static bool scheduleTimerHandler(void)
{
    Thread* curThread = Scheduler::GetInstance()->GetCurrentThread();
    curThread->SetTimeslice(curThread->GetTimeslice() - SCHED_TIMER_FREQUENCY);
    
    SCHEDULER_DEBUG_MSG("Timeslice of current thread: " << dec << (unsigned)curThread->GetTimeslice());
    
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

static bool threadLessThan(Thread* UNUSED(t1), Thread* UNUSED(t2))
{
    //just return true to get the old thread positioned
    //at the end of the queue for now
    return true;
}

Scheduler::Scheduler()
{
    DisableInterrupts();
    
    threadQueue = new OrderedArray<Thread*, 128>();
    threadQueue->SetPredicate(threadLessThan);
    
    kernelThread = new Thread(0, 0, 0, 0, "Kernel thread");
    currentThread = kernelThread;
    threadQueue->Insert(currentThread);
    
    schedulingTimer = new Timer(FUNCTION, scheduleTimerHandler, NULL);
    
    tm = NULL;
    nextId = 1;
    
    SCHEDULER_DEBUG_MSG("Scheduler initialized");
    
    EnableInterrupts();
}

void Scheduler::SetTimerManager(TimerManager* t)
{
    tm = t;
    currentThread->SetTimeslice(SCHED_THREAD_TIMESLICE);
    tm->StartTimer(schedulingTimer, SCHED_TIMER_FREQUENCY);
}

void Scheduler::AddThread(Thread* thread)
{
    threadQueue->Insert(thread);
}

void Scheduler::Schedule(registers_t* oldState)
{
    SCHEDULER_DEBUG_MSG("Entering scheduler");
    DisableInterrupts();
    
    saveThreadInfo(&currentThread->threadInfo, oldState);
    
    //check if we have a timer manager
    ASSERT(tm != NULL, "TimerManager is NULL");
    
    if (threadQueue->GetSize() == 0)
        return;
    
    Thread *next = threadQueue->ItemAt(0);
	threadQueue->RemoveAt(0);
	threadQueue->Insert(next);
	SCHEDULER_DEBUG_MSG("Picking thread " << next->GetName());
	currentThread = next;
    
    currentThread->SetTimeslice(SCHED_THREAD_TIMESLICE);
    tm->StartTimer(schedulingTimer, SCHED_TIMER_FREQUENCY);
    
    printThreadInfo(&currentThread->threadInfo);
    
    switchToThread(&currentThread->threadInfo);
}

void Scheduler::DumpThreads(CharacterOutputDevice& c)
{
    for(unsigned int i = 0; i < threadQueue->GetSize(); i++)
    {
        Thread *t = threadQueue->ItemAt(i);
        c << "SCHEDULER: " << "\tThread ID: " << dec << t->GetId() << endl;
        c << "SCHEDULER: " << "\tThread Name: " << t->GetName() << endl;
        c << "SCHEDULER: " << "\tInstruction Pointer: " << hex << (unsigned)t->GetInstructionPointer() << endl;
        c << "SCHEDULER: " << "\tStack Pointer: " << (unsigned)t->GetStackPointer() << endl;
        c << "SCHEDULER: " << "\tBase Pointer: " << (unsigned)t->GetBasePointer() << endl;
        c << "SCHEDULER: " << "\tTimeslice: " << dec << (unsigned)t->GetTimeslice() << endl;
        
        c << endl;
    }
}
