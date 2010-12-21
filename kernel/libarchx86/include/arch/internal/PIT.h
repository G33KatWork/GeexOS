#ifndef _ARCHX86_PIT_H
#define _ARCHX86_PIT_H

#include <types.h>
#include <arch/types.h>
#include <halinterface/BaseTimer.h>
#include <halinterface/HAL.h>

namespace Arch
{
    class PIT : public BaseTimer
    {
    private:
        unsigned long curTickLen;
        
        uint16_t GetCurrentCount();
        
    public:
        PIT() { curTickLen = 0; }
        
        virtual const char* GetName() { return "PIT"; }
        virtual ClockType GetClockType() { return CLKTYPE_PERIODIC; }
        virtual unsigned long GetTickLengthUs() { return curTickLen; }
        
        virtual void SetTickLengthUs(unsigned long us);
        
        /// Spin-waits for the given amount of time
        virtual void WaitForTick();
        
        virtual void EnableInterrupt() { CurrentHAL->GetInterruptDispatcher()->UnmaskInterrupt(BaseInterruptDispatcher::IRDEV_TIMER); }
        virtual void DisableInterrupt() { CurrentHAL->GetInterruptDispatcher()->MaskInterrupt(BaseInterruptDispatcher::IRDEV_TIMER); }
    };
}

#endif
