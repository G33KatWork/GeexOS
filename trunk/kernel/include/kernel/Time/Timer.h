#ifndef _TIMER_H
#define _TIMER_H

#include <lib/types.h>
#include <arch/clock_source.h>
#include <kernel/Processes/Process.h>

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
        Process *process;
        
    public:
        Timer(TimerAction act, TimerFunction func, Process *proc)
        {
            this->length = 0;
            this->function = func;
            this->process = proc;
            this->action = act;
        }
        
        void timerExpired();
        
        void SetLength(unsigned long len) { length = len; }
        unsigned long GetLength() { return length; }
    };
}

#endif
