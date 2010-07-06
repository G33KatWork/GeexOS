#ifndef _TIMER_MANAGER_H
#define _TIMER_MANAGER_H

#include <types.h>
#include <halinterface/ClockSource.h>
#include <kernel/Time/Timer.h>
#include <kernel/DataStructures/List.h>

using namespace Arch;
using namespace DataStructures;

namespace Time
{
    class TimerManager {
    private:
        ClockSource *clockSource;
        unsigned long tickLen;
        List<Timer*> *currentTimers;
        
        void prepareClock(unsigned long ns);
        
    public:
        TimerManager(ClockSource* source)
        {
            clockSource = NULL;
            currentTimers = new List<Timer*>();
            SetClockSource(source);
        }
        
        void SetClockSource(ClockSource *newSource);
        
        bool HandleTick(ClockSource *source);
        void StartTimer(Timer* t, unsigned long length);
        void StopTimer(Timer* t);
    };
}

#endif
