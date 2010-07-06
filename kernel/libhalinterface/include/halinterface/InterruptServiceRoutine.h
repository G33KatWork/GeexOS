#ifndef _HAL_INTERFACES_INTERRUPT_SERVICE_ROUTINE_H
#define _HAL_INTERFACES_INTERRUPT_SERVICE_ROUTINE_H

#include <types.h>

namespace Arch
{
    //Interface for ISRs
    class InterruptServiceRoutine
    {
    protected:
        virtual ~InterruptServiceRoutine() {};

    public:    
        virtual void Execute(registers_t *regs) = 0;
    };
}

#endif
