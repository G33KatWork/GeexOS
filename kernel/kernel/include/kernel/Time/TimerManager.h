#ifndef _TIMER_MANAGER_H
#define _TIMER_MANAGER_H

#include <halinterface/ClockSource.h>
#include <kernel/Time/Timer.h>
#include <kernel/DataStructures/DoublyLinkedList.h>

namespace Time
{
    class TimerManager {
    private:
        Arch::ClockSource *clockSource;
        unsigned long tickLen;
        DataStructures::DoublyLinkedList<Timer> currentTimers;
        
        void prepareClock(unsigned long us);
        
    public:
        TimerManager(ClockSource* source)
        {
            clockSource = NULL;
            SetClockSource(source);
        }
        
        void SetClockSource(ClockSource *newSource);
        
        bool HandleTick(ClockSource *source);
        void StartTimer(Timer* t, unsigned long length);
        void StopTimer(Timer* t);
    };
}

#endif
