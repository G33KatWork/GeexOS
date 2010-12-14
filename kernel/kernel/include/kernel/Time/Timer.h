#ifndef _TIMER_H
#define _TIMER_H

#include <halinterface/ClockSource.h>
#include <kernel/Processes/Thread.h>
#include <kernel/DataStructures/DoublyLinkedList.h>

namespace Time
{
    class Timer;
    
    enum TimerAction {
        PREEMPT,
        WAKEUP,
        FUNCTION,
        OOPCALLBACK
    };
    
    class ITimerCallback {
    public:
        virtual bool TimerExpired(Timer* t) = 0;
    };
    
    typedef bool (*TimerFunction)(Timer* timer);
    
    class Timer : public DataStructures::DoublyLinkedListLinkImpl<Timer>
    {
    private:
        unsigned long length;
        TimerAction action;
        TimerFunction function;
        Processes::Thread *thread;
        ITimerCallback *object;
        
    public:
        Timer(TimerAction act, TimerFunction func, Processes::Thread *t, ITimerCallback* callbackObject)
        {
            this->length = 0;
            this->function = func;
            this->thread = t;
            this->action = act;
            this->object = callbackObject;
        }
        
        bool timerExpired();
        
        void SetLength(unsigned long len) { length = len; }
        unsigned long GetLength() { return length; }
    };
}

#endif
