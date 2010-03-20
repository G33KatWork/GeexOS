#ifndef TIMERHANDLER_H_
#define TIMERHANDLER_H_

#include <lib/types.h>
#include <kernel/IInterruptServiceRoutine.h>
#include <kernel/Time/TimerManager.h>

using namespace Time;

class TimerHandler : public IInterruptServiceRoutine
{
private:
    TimerManager *tm;
    
public:
    TimerHandler(TimerManager* timerManager)
    {
        tm = timerManager;
    }
    
    void Execute(registers_t* UNUSED(regs))
    {
        this->tm->HandleTick(&Arch::ClockSource);
        /*if(this->tm->HandleTick(&Arch::ClockSource))
            Scheduler::GetInstance()->Schedule();*/
    }
};

#endif
