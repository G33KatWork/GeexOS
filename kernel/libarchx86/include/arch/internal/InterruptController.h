#ifndef _HAL_INTERFACES_BASEINTERRUPTCONTROLLER_H
#define _HAL_INTERFACES_BASEINTERRUPTCONTROLLER_H

#include <arch/types.h>

namespace Arch
{   
    /**
     * This is the base class for an interrupt controller on a platform.
     * It handles the low level interrupt handling i.e. configuring the interrupt controller hardware
    **/
    class InterruptController
    {
    protected:
        InterruptController() {}
        virtual ~InterruptController() {}
    
    public:
        virtual void Initialize() = 0;
        
        virtual void MaskVector(uint8_t vectorNumber) = 0;
        virtual void UnmaskVector(uint8_t vectorNumber) = 0;
        
        virtual bool EndOfInterrupt(uint8_t vectorNumber) = 0;
    };
}

#endif
