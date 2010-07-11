#ifndef _HAL_INTERFACES_IINTERRUPTDISPATCHER_H
#define _HAL_INTERFACES_IINTERRUPTDISPATCHER_H

#include <halinterface/InterruptServiceRoutine.h>

namespace Arch
{   
    class BaseInterruptDispatcher : public InterruptServiceRoutine
    {
    public:
        enum Exception
        {
            EXC_DIVBY0,
            EXC_DEBUG,
            EXC_NMI,
            EXC_BREAKPOINT,
            EXC_OVERFLOW,
            EXC_BOUNDCHECK,
            EXC_INVALIDOPCODE,
            EXC_COPROCNOTAVAIL,
            EXC_DOUBLEFAULT,
            EXC_COPROCSEGMENTOVERRUN,
            EXC_INVALIDTSS,
            EXC_SEGMENTNOTPRESENT,
            EXC_STACK,
            EXC_GENERALPROTECTIONFAULT,
            EXC_PAGEFAULT,
            EXC_COPROCERROR
        };
        
        enum InterruptingDevice
        {
            IRDEV_TIMER,
            IRDEV_KEYBOARD,
            IRDEV_SERIAL1,
            IRDEV_SERIAL2,
            IRDEV_PARALLEL2,
            IRDEV_FLOPPY,
            IRDEV_PARALLEL1,
            IRDEV_RTC,
            IRDEV_PS2MOUSE,
            IRDEV_FPU,
            IRDEV_ATAPRIMARY,
            IRDEV_ATASLAVE
        };
    
    protected:
        BaseInterruptDispatcher() {};
        virtual ~BaseInterruptDispatcher() {};
    
    public:
        virtual bool RegisterInterruptHandler(InterruptingDevice i, InterruptServiceRoutine* isr) = 0;
        virtual bool UnregisterInterruptHandler(InterruptingDevice i) = 0;
        
        virtual bool RegisterExceptionHandler(Exception e, InterruptServiceRoutine* isr) = 0;
        virtual bool UnregisterExceptionHandler(Exception e) = 0;
        
        virtual void MaskInterrupt(InterruptingDevice i) = 0;
        virtual void UnmaskInterrupt(InterruptingDevice i) = 0;
        
        virtual void Execute(registers_t *regs) = 0;
    };
}

#endif
