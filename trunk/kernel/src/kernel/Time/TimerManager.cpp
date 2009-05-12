#include <kernel/global.h>
#include <kernel/Time/Timer.h>
#include <kernel/Time/TimerManager.h>
#include <kernel/DataStructures/List.h>

using namespace Time;
using namespace DataStructures;

#include <kernel/global.h>
using namespace IO;

void TimerManager::prepareClock(unsigned long ns)
{
    if(clockSource->type == PERIODIC)
        return;
    
    tickLen = ns;
    clockSource->prepare(ns);
}

void TimerManager::SetClockSource(ClockSource_t *newSource)
{
    if(clockSource != NULL)
    {
        clockSource->disable();
    }
    
    clockSource = newSource;
    
    if(clockSource->type == PERIODIC)
    {
        tickLen = clockSource->tickLength;
        clockSource->enable();
    }
    else
    {
        clockSource->enable();
        prepareClock(clockSource->tickLength);
    }
}

void TimerManager::HandleTick(ClockSource_t *source)
{
    if(source != clockSource)
        PANIC("Unexpected clock source!");
    
    int i = 0;
    Timer *t = currentTimers->getAt(i);
    
    while(t != NULL)
    {
        if(tickLen < t->GetLength())
        {
            t->SetLength(t->GetLength() - tickLen); //decrement time until timer expires
            
            i++;
            t = currentTimers->getAt(i);
            
            continue;
        }
        
        //if we get here, the timer has already expired
        currentTimers->remove(t);
        t->SetLength(0);
        t->timerExpired();
        delete t;
        
        //get next timer from list
        t = currentTimers->getAt(i);
    }
    
    prepareClock(tickLen);
}

void TimerManager::StartTimer(Timer* t, unsigned long length)
{
    t->SetLength(length);
    currentTimers->append(t);
}

void TimerManager::StopTimer(Timer* t)
{
    currentTimers->remove(t);
}
