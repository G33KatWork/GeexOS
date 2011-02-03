#ifndef _ARCHX86_IINTERRUPTDISPATCHER_H
#define _ARCHX86_IINTERRUPTDISPATCHER_H

#include <types.h>
#include <halinterface/BaseInterruptDispatcher.h>
#include <arch/internal/idt.h>

#define     ARCH_X86_EXCEPTION_BASE         0
#define     ARCH_X86_INTERRUPT_BASE         0x20
#define     ARCH_X86_LOCAL_INTERRUPT_BASE   0xfb

// Set the following number to the maximum nested exceptions
// the kernel tries to resolve before it will panic
#define     MAX_NESTED_EXCEPTIONS   1

//Handler called by our asm stubs
extern "C" void fault_handler(registers_t regs);
extern "C" void interrupt_handler(registers_t regs);

namespace Arch
{   
    class x86InterruptDispatcher : public BaseInterruptDispatcher
    {
    public:
        x86InterruptDispatcher();
        virtual ~x86InterruptDispatcher() {};
    
        virtual bool RegisterInterruptHandler(InterruptingDevice i, InterruptServiceRoutine* isr);
        virtual bool UnregisterInterruptHandler(InterruptingDevice i);
        
        virtual bool RegisterExceptionHandler(Exception e, InterruptServiceRoutine* isr);
        virtual bool UnregisterExceptionHandler(Exception e);
        
        virtual void MaskInterrupt(InterruptingDevice i);
        virtual void UnmaskInterrupt(InterruptingDevice i);
        
        virtual void Execute(registers_t *regs);
    
    private:
        int exceptionToVector(Exception e);
        int interruptToVector(InterruptingDevice i);
        
        InterruptServiceRoutine* isrs[I86_IDT_MAX_ENTRY_COUNT];
    };
}

#endif
