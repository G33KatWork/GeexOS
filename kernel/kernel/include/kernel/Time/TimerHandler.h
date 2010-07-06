#ifndef TIMERHANDLER_H_
#define TIMERHANDLER_H_

#include <types.h>
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
        //if(this->tm->HandleTick(&Arch::ClockSource))
            s->Schedule(regs);
    }
};

#endif
