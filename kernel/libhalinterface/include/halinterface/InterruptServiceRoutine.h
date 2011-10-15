#ifndef _HAL_INTERFACES_INTERRUPT_SERVICE_ROUTINE_H
#define _HAL_INTERFACES_INTERRUPT_SERVICE_ROUTINE_H

namespace Arch
{
    //Interface for ISRs
    class InterruptServiceRoutine
    {
    protected:
        virtual ~InterruptServiceRoutine() {};

    public:    
        virtual void Execute() = 0;
    };
}

#endif
