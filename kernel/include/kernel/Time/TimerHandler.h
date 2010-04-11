#ifndef TIMERHANDLER_H_
#define TIMERHANDLER_H_

#include <lib/types.h>
#include <kernel/IInterruptServiceRoutine.h>
#include <kernel/Time/TimerManager.h>
#include <kernel/Processes/Scheduler.h>

#include <kernel/debug.h>

using namespace Time;
using namespace Processes;

class TimerHandler : public IInterruptServiceRoutine
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
        if(this->tm->HandleTick(&Arch::ClockSource))
            s->Schedule(regs);
    }
};

#endif
