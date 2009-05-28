#include <kernel/Processes/Scheduler.h>
#include <kernel/Time/TimerManager.h>

using namespace Processes;
using namespace Arch;
using namespace Time;

static bool scheduleTimerHandler(void)
{
    Scheduler::GetInstance()->GetCurrentThread()->SetTimeslice(0);
    return true; //force scheduling
}

Scheduler* Scheduler::instance = NULL;

Scheduler* Scheduler::GetInstance()
{
    if(instance == NULL)
        instance = new Scheduler();
    
    return instance;
}

static bool threadLessThan(Thread *t1, Thread *t2)
{
    //just return true to get the old thread positioned
    //at the end of the queue for now
    return true;
}

Scheduler::Scheduler()
{
    threadQueue = new OrderedArray<Thread*, 128>();
    threadQueue->SetPredicate(threadLessThan);
    
    //TODO: Last argument is pagedirectory!!!
    kernelThread = new Thread(NULL, (void *)NULL, 1, NULL);
    currentThread = kernelThread;
    
    schedulingTimer = new Timer(FUNCTION, scheduleTimerHandler, NULL);
    
    tm = NULL;
    
    DEBUG_MSG("Scheduler initialized");
}

void Scheduler::Schedule()
{   
    DisableInterrupts();
    
    //check if we have a timer manager
    ASSERT(tm != NULL, "TimerManager is NULL");
    
    if (threadQueue->GetSize())
    {
        //TODO: use several queues for priorities
        Thread *next = threadQueue->ItemAt(0);
        Thread *old = currentThread;
        threadQueue->RemoveAt(0);
        
        //TODO: Check status if zombie, blocked etc. and schedule only runnable tasks
        threadQueue->Insert(currentThread);
        
        //Switch
        currentThread = next;
        //kout << "Switching to task: " << dec << next->GetId();
        
        //Calculate new timeslice (in nanoseconds)
        //TODO: use real algorithm...
        currentThread->SetTimeslice(10000000);
        tm->StartTimer(schedulingTimer, 10000000);
        
        if(old != currentThread)
        {
            currentThread->GetContext()->SwitchTo(old->GetContext());
        }
    }
    
    //Add your code that should be executed after a wakeup here
    //kout << "post";
    
    EnableInterrupts();
}

void Scheduler::AddThread(Thread* t)
{
    threadQueue->Insert(t);
}

