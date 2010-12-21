#ifndef _TIMER_MANAGER_H
#define _TIMER_MANAGER_H

#include <halinterface/BaseTimer.h>
#include <kernel/Time/Timer.h>
#include <kernel/DataStructures/DoublyLinkedList.h>

namespace Time
{
    class TimerManager {
    private:
        Arch::BaseTimer *hardwareTimer;
        unsigned long tickLen;
        DataStructures::DoublyLinkedList<Timer> currentTimers;
        
        void prepareClock();
        
    public:
        TimerManager(Arch::BaseTimer* source)
        {
            hardwareTimer = NULL;
            SetClockSource(source);
        }
        
        void SetClockSource(Arch::BaseTimer* newSource);
        
        bool HandleTick(Arch::BaseTimer* source);
        void StartTimer(Timer* t, unsigned long length);
        void StopTimer(Timer* t);
    };
}

#endif
