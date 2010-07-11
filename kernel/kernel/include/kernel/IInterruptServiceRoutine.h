#ifndef _INTERRUPT_SERVICE_ROUTINE_H
#define _INTERRUPT_SERVICE_ROUTINE_H

#include <arch/types.h>

namespace Kernel
{
    //Interface for ISRs
    class IInterruptServiceRoutine
    {
    protected:
        virtual ~IInterruptServiceRoutine() {};

    public:    
        virtual void Execute(registers_t *regs) = 0;
    };
}

#endif
