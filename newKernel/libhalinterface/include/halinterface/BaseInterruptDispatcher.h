#ifndef _HAL_INTERFACES_IINTERRUPTDISPATCHER_H
#define _HAL_INTERFACES_IINTERRUPTDISPATCHER_H

#include <halinterface/InterruptServiceRoutine.h>

namespace Arch
{   
    /**
     * This is the base class for an interrupt dispatcher living in the HAL.
     * This dispatcher is responsible for registering a given callback with a
     * platform specific vector number.
     * Note that this is not the interrupt controller which lives inside the platform driver!
     * It is only for dispatching a hardware dependent IRQ to some higher level handler.
     * A HAL implementation has to inherit from this abstract class to
     * provide general interrupt dispatching for the operating system.
    **/
    class BaseInterruptDispatcher
    {
    protected:
        BaseInterruptDispatcher() {};
        virtual ~BaseInterruptDispatcher() {};
    
    public:
        virtual bool RegisterInterruptVector(uint32_t vector, InterruptServiceRoutine* isr) = 0;
        virtual bool UnregisterInterruptVector(uint32_t vector) = 0;
    };
}

#endif
