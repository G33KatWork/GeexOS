#include <kernel/global.h>
#include <kernel/Time/Timer.h>
#include <kernel/Time/TimerManager.h>

using namespace Arch;
using namespace Time;
using namespace Debug;

void TimerManager::prepareClock()
{
    if(hardwareTimer->GetClockType() == CLKTYPE_PERIODIC)
        return;
    
    hardwareTimer->PrepareOneSot();
}

void TimerManager::SetClockSource(Arch::BaseTimer* newSource)
{
    if(hardwareTimer != NULL)
        hardwareTimer->DisableInterrupt();
    
    hardwareTimer = newSource;
    
    tickLen = hardwareTimer->GetTickLengthUs();
    
    if(hardwareTimer->GetClockType() == CLKTYPE_PERIODIC)
        hardwareTimer->EnableInterrupt();
    else
    {
        hardwareTimer->EnableInterrupt();
        prepareClock();
    }
}

bool TimerManager::HandleTick(Arch::BaseTimer* source)
{
    bool needScheduling = false;
    
    if(source != hardwareTimer)
        PANIC("Unexpected clock source!");
    
    Timer *t = currentTimers.Head();
    while(t != NULL)
    {
        if(tickLen < t->GetLength() * 1000)  //convert miliseconds from timer to microsenconds (*1000)
        {
            t->SetLength(t->GetLength() - tickLen); //decrement time until timer expires
            TIMER_MGR_DEBUG_MSG("Drecemented remaining time of timer to " << dec << (unsigned int)t->GetLength());
            
            t = currentTimers.GetNext(t);
            continue;
        }
        
        //if we get here, the timer has already expired
        
        //remove current timer, since it is expired
        Timer* nextLoopTimer = currentTimers.GetNext(t);
        currentTimers.Remove(t);
        
        t->SetLength(0);
        bool ret = t->timerExpired();
        TIMER_MGR_DEBUG_MSG("A timer expired");
        
        if(!needScheduling)
            needScheduling = ret;
        
        t = nextLoopTimer;
    }
    
    prepareClock();
    
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
    currentTimers.Append(t);
}

void TimerManager::StopTimer(Timer* t)
{
    currentTimers.Remove(t);
}
