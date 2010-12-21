#ifndef _HAL_INTERFACES_IINTERRUPTDISPATCHER_H
#define _HAL_INTERFACES_IINTERRUPTDISPATCHER_H

#include <halinterface/InterruptServiceRoutine.h>

namespace Arch
{   
    /**
     * This is the base class for an interrupt handler living in the HAL.
     * A HAL implementation has to inherit from this abstract class to
     * provide general interrupt handling for the operating system.
    **/
    class BaseInterruptDispatcher : public InterruptServiceRoutine
    {
    public:
        /**
         * This enumeration holds possible exception types a processor
         * could raise. The kernel can register an ExceptionHandler with
         * RegisterExceptionHandler. Unregistering is possible with
         * UnregisterExceptionHandler.
        **/
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
        
        /**
         * This enumeration holds possible device types which could be available
         * on a platform. The HAL is responsible for allocating the real interrupt
         * numbers to these device types when a handler is registered with 
         * RegisterInterruptHandler. Unregistering is possible with 
         * UnregisterExceptionHandler.
        **/
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
        
        enum IRQLevel
        {
            IRQL_HIGH,                  ///< Used at system crash to mask all IRQs
            IRQL_IPI,                   ///< Inter-Processor-Interrupt used for signalling TLB FLush, Crash, etc.
            IRQL_CLOCK,                 ///< Timer interrupt
            IRQL_DEVICE_IMPORTANT,      ///< Highest device priority interrupt
            IRQL_DEVICE_LESSIMPORTANT,  ///< 2nd highest device priority interrupt
            IRQL_DEVICE_NORMAL,         ///< Normal device priority interrupt
            IRQL_DEVICE_UNIMPORTANT,    ///< Lowest device priority interrupt
            IRQL_MESSAGE,               ///< Software interrupt which handles message receiving and sending
            IRQL_PASSIVE                ///< No interrupt is currently handled (processor is idling or in usermode)
        };
    
    protected:
        BaseInterruptDispatcher() {};
        virtual ~BaseInterruptDispatcher() {};
    
    public:
        virtual bool RegisterInterruptHandler(InterruptingDevice i, InterruptServiceRoutine* isr) = 0;
        virtual bool UnregisterInterruptHandler(InterruptingDevice i) = 0;
        
        virtual bool RegisterExceptionHandler(Exception e, InterruptServiceRoutine* isr) = 0;
        virtual bool UnregisterExceptionHandler(Exception e) = 0;
        
        /**
         * This call masks an interrupt of a specific device.
         * If an interrupt is masked, it means that the corresponding handler
         * isn't called anymore. All succeeding interrupts are lost.
        **/ 
        virtual void MaskInterrupt(InterruptingDevice i) = 0;
        
        /**
         * Unmasking an interrupt reenables the handling of it.
         * After calling this method all succeeding interrupts are
         * delivered to the handler.
        **/
        virtual void UnmaskInterrupt(InterruptingDevice i) = 0;
        
        /**
         * This method should only be called from an interrupt context (i.e. the low level interrupt handlers).
         * It dispatches an occured interrupt to a registered handler or discards it.
        **/
        virtual void Execute(registers_t *regs) = 0;
    };
}

#endif
