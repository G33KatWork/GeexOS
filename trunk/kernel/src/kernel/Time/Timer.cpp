#include <kernel/global.h>
#include <kernel/Time/Timer.h>

using namespace Time;

void Timer::timerExpired()
{
    switch(action)
    {
        case PREEMPT:
        break;
        
        case WAKEUP:
        break;
        
        case FUNCTION:
            if(function != NULL)
                function();
            break;
    }
}
