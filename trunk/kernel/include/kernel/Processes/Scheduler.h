#ifndef PROCESSES_SCHEDULER_H_
#define PROCESSES_SCHEDULER_H_

#include <lib/types.h>
#include <kernel/Processes/Thread.h>
#include <arch/CPUContext.h>
#include <kernel/DataStructures/OrderedArray.h>
#include <kernel/Time/Timer.h>
#include <kernel/Time/TimerManager.h>

using namespace DataStructures;
using namespace Time;

namespace Processes
{
    class Scheduler
    {
    public:
        static Scheduler* GetInstance();
        
        void Schedule();
        void AddThread(Thread* t);
        
        void SetTimerManager(TimerManager* t) { tm = t; }
        
        Thread* GetCurrentThread() { return currentThread; }
        
    private:
        static Scheduler* instance;
        
        //FIXME: Use dynamic array
        OrderedArray<Thread*, 128> *threadQueue;
        Thread* currentThread;
        Thread* kernelThread;
        
        Timer* schedulingTimer;
        TimerManager* tm;
        
        Scheduler();
    };
}
#endif
