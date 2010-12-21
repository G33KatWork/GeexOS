#ifndef _HAL_INTERFACES_BASETIMER_H
#define _HAL_INTERFACES_BASETIMER_H

#include <types.h>

namespace Arch
{
    enum ClockType {
        CLKTYPE_PERIODIC,
        CLKTYPE_ONESHOT
    };
    
    /**
     * This is the base class for a timer on a platform.
     * Each timer in a platform needs a driver which inherits from this class.
     * The current active timer can be retrieved with HAL::GetHardwareClockSource().
    **/
    class BaseTimer
    {
    protected:
        BaseTimer() {}
        virtual ~BaseTimer() {}
    
    public:
        /// Returns the name of the Timer
        virtual const char* GetName() = 0;
        
        /// Returns the current time between 2 consecutive ticks in nanoseconds
        virtual unsigned long GetTickLengthUs() = 0;
        
        /// Configures the timer to the given tick length in nanoseconds
        virtual void SetTickLengthUs(unsigned long us) = 0; 
        
        virtual ClockType GetClockType() = 0;
        
        /// Prepares a one-shot timer. It fires after the given time by SetTickLengthUs. Only valid if Clock Type is CLOCKTYPE_ONESHOT
        virtual void PrepareOneSot() {};
        
        /// Spin-waits for the given amount of time
        virtual void WaitForTick() = 0;
        
        virtual void EnableInterrupt() = 0;
        virtual void DisableInterrupt() = 0;
    };
}

#endif
