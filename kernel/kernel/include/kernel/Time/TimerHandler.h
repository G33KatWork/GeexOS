#ifndef TIMERHANDLER_H_
#define TIMERHANDLER_H_

#include <halinterface/InterruptServiceRoutine.h>
#include <kernel/Time/TimerManager.h>
#include <kernel/Processes/Scheduler.h>

#include <kernel/debug.h>

using namespace Time;
using namespace Processes;

class TimerHandler : public InterruptServiceRoutine
{
private:
    TimerManager *tm;
    Scheduler *s;
    
public:
    TimerHandler(TimerManager* timerManager, Scheduler* scheduler)
    {
        tm = timerManager;
        s = scheduler;
    }
    
    void Execute(registers_t* regs)
    {
        //DEBUG_MSG("tick");
        if(this->tm->HandleTick(CurrentHAL->GetHardwareClockSource()))
        {
            //DEBUG_MSG("Scheduling");
            s->Schedule(regs);
        }
        else
        {
            //DEBUG_MSG("Not Scheduling");
        }
    }
};

#endif
