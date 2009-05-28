#include <kernel/global.h>
#include <kernel/Time/Timer.h>

using namespace Time;

bool Timer::timerExpired()
{
    bool ret = false;
    
    switch(action)
    {
        case PREEMPT:
        break;
        
        case WAKEUP:
        break;
        
        case FUNCTION:
            if(function != NULL)
                ret = function();
            break;
    }
    
    return ret;
}
