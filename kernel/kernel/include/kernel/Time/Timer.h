#ifndef _TIMER_H
#define _TIMER_H

#include <types.h>
#include <arch/clock_source.h>
#include <kernel/Processes/Thread.h>

using namespace Arch;
using namespace Processes;

namespace Time
{
    enum TimerAction {
        PREEMPT,
        WAKEUP,
        FUNCTION
    };
    
    typedef bool (*TimerFunction)(void);
    
    class Timer {
    private:
        unsigned long length;
        TimerAction action;
        TimerFunction function;
        Thread *thread;
        
    public:
        Timer(TimerAction act, TimerFunction func, Thread *t)
        {
            this->length = 0;
            this->function = func;
            this->thread = t;
            this->action = act;
        }
        
        bool timerExpired();
        
        void SetLength(unsigned long len) { length = len; }
        unsigned long GetLength() { return length; }
    };
}

#endif
