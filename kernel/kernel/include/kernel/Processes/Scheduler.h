#ifndef PROCESSES_SCHEDULER_H_
#define PROCESSES_SCHEDULER_H_

#include <kernel/Processes/Thread.h>
#include <kernel/DataStructures/OrderedArray.h>
#include <kernel/Time/Timer.h>
#include <kernel/Time/TimerManager.h>
#include <halinterface/BaseDebugOutputDevice.h>

using namespace DataStructures;
using namespace Time;

namespace Processes
{
    class Scheduler
    {
    public:
        static Scheduler* GetInstance();
        
        void Schedule(registers_t* oldState);
        void SetTimerManager(TimerManager* t);
        
        Thread* GetCurrentThread() { return currentThread; }
        
        void DumpThreads(Debug::BaseDebugOutputDevice* c);
        
        void AddThread(Thread* thread);
        
    private:
        static Scheduler* instance;
        
        Thread* listHead;
        Thread* currentThread;
        Thread* kernelThread;
        
        //Timer* schedulingTimer;
        TimerManager* tm;
        
        unsigned int nextId;
        
        Scheduler();
    };
}
#endif
