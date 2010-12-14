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
    class Scheduler : public ITimerCallback
    {
    public:
        /// Returns the singleton instance
        static Scheduler* GetInstance();
        
        /**
         * Reschedules a thread.
         * This function should only be called by an interrupt handler
        **/
        void Schedule(registers_t* oldState);
        
        /**
         * Sets the timer manager to be used by the scheduler
         * The scheduler creates a timer inside this manager. If it expires,
         * it is calls the Schedule() method to select a new thread to run
        **/
        void SetTimerManager(TimerManager* t);
        
        /// Returns a pointer to the current running thread
        Thread* GetCurrentThread() { return currentThread; }
        
        /// Prints a list of all enqueued threads on the given output device
        void DumpThreads(Debug::BaseDebugOutputDevice* c);
        
        /// Enqueues a thread into the running queue
        void AddThread(Thread* thread);
        
        /// Implementation of ITimerCallback for timer expiry
        virtual bool TimerExpired(Timer* t);
        
    private:
        static Scheduler* instance;
        
        Thread* listHead;
        Thread* currentThread;
        Thread* kernelThread;
        
        Timer* schedulingTimer;
        TimerManager* tm;
        
        unsigned int nextId;
        
        Scheduler();
    };
}
#endif
