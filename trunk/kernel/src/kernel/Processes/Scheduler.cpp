#include <kernel/Processes/Scheduler.h>
#include <kernel/Time/TimerManager.h>
#include <arch/scheduling.h>

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
    kernelThread = new Thread(1, NULL);
    currentThread = kernelThread;
    
    schedulingTimer = new Timer(FUNCTION, scheduleTimerHandler, NULL);
    
    tm = NULL;
    
    //DEBUG_MSG("Scheduler initialized");
}

void Scheduler::Schedule()
{   
    DisableInterrupts();
    
    //check if we have a timer manager
    ASSERT(tm != NULL, "TimerManager is NULL");
    
    if (threadQueue->GetSize())
    {
		currentThread->SetStackPointer(readStackPointer());
		currentThread->SetBasePointer(readBasePointer());
		Address instructionPointer = readInstructionPointer();
		
		//If IP is 0x20 we just returned from a context switch...
		if(instructionPointer == 0x20)
		{
			return;
		}
	
		currentThread->SetInstructionPointer(instructionPointer);
		
		//Enqueue current thread
        threadQueue->Insert(currentThread);

		//Get next thread
		//TODO: Check status if zombie, blocked etc. and schedule only runnable tasks
		Thread *next = threadQueue->ItemAt(0);
		threadQueue->RemoveAt(0);
		currentThread = next;
	
        //Calculate new timeslice (in nanoseconds)
        //TODO: use real algorithm...
        currentThread->SetTimeslice(10000000);
        tm->StartTimer(schedulingTimer, 10000000);

		currentThread->SwitchTo();
    }
    
    //Add your code that should be executed after a wakeup here
    //kout << "post";
    
    EnableInterrupts();
}

int Scheduler::Fork()
{
    //TODO: Implement fork()
    return 0;
}

