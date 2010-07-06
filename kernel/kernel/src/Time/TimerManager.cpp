#include <kernel/global.h>
#include <kernel/Time/Timer.h>
#include <kernel/Time/TimerManager.h>
#include <kernel/DataStructures/List.h>

using namespace Time;
using namespace DataStructures;

#include <kernel/debug.h>
using namespace IO;

void TimerManager::prepareClock(unsigned long ns)
{
    if(clockSource->type == CLKTYPE_PERIODIC)
        return;
    
    tickLen = ns;
    clockSource->prepare(ns);
}

void TimerManager::SetClockSource(ClockSource *newSource)
{
    if(clockSource != NULL)
    {
        clockSource->disable();
    }
    
    clockSource = newSource;
    
    if(clockSource->type == CLKTYPE_PERIODIC)
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

bool TimerManager::HandleTick(ClockSource *source)
{
    bool needScheduling = false;
    
    if(source != clockSource)
        PANIC("Unexpected clock source!");
    //kdbg << "TickTack" << endl;
    int i = 0;
    Timer *t = currentTimers->getAt(i);
    //kdbg << (t == NULL ? "t = NULL" : " t != NULL") << endl;
    
    while(t != NULL)
    {
        if(tickLen < t->GetLength())
        {
            t->SetLength(t->GetLength() - tickLen); //decrement time until timer expires
            TIMER_MGR_DEBUG_MSG("Drecemented remaining time of timer to " << dec << (unsigned int)t->GetLength());
            
            i++;
            t = currentTimers->getAt(i);
            continue;
        }
        
        //if we get here, the timer has already expired
        currentTimers->remove(t);
        t->SetLength(0);
        bool ret = t->timerExpired();
        TIMER_MGR_DEBUG_MSG("A timer expired");
        
        if(!needScheduling)
            needScheduling = ret;
        
        //get next timer from list
        t = currentTimers->getAt(i);
    }
    
    prepareClock(tickLen);
    
    #ifdef EN_TIMER_MGR_DEBUG_MSG
    if(needScheduling)
        TIMER_MGR_DEBUG_MSG("At least one timer requested scheduling");
    #endif
    
    return needScheduling;
}

void TimerManager::StartTimer(Timer* t, unsigned long length)
{
    TIMER_MGR_DEBUG_MSG("A new timer with length of " << length << " was started");
    t->SetLength(length);
    currentTimers->append(t);
}

void TimerManager::StopTimer(Timer* t)
{
    currentTimers->remove(t);
}
