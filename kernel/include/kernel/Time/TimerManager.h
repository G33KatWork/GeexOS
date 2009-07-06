#ifndef _TIMER_MANAGER_H
#define _TIMER_MANAGER_H

#include <lib/types.h>
#include <arch/clock_source.h>
#include <kernel/Time/Timer.h>
#include <kernel/DataStructures/List.h>

using namespace Arch;
using namespace DataStructures;

namespace Time
{
    class TimerManager {
    private:
        ClockSource_t *clockSource;
        unsigned long tickLen;
        List<Timer*> *currentTimers;
        
        void prepareClock(unsigned long ns);
        
    public:
        TimerManager(ClockSource_t* source)
        {
            clockSource = NULL;
            currentTimers = new List<Timer*>();
            SetClockSource(source);
        }
        
        void SetClockSource(ClockSource_t *newSource);
        
        bool HandleTick(ClockSource_t *source);
        void StartTimer(Timer* t, unsigned long length);
        void StopTimer(Timer* t);
    };
}

#endif
