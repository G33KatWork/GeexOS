#ifndef _ARCHX86_IINTERRUPTDISPATCHER_H
#define _ARCHX86_IINTERRUPTDISPATCHER_H

#include <types.h>
#include <halinterface/BaseInterruptDispatcher.h>
#include <arch/internal/IDT.h>

#include <arch/internal/registers.h>

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
    
        virtual bool RegisterInterruptVector(uint32_t vector, InterruptServiceRoutine* isr);
        virtual bool UnregisterInterruptVector(uint32_t vector);
        
        void Execute(registers_t* regs, bool isException);
    
    private:
        InterruptServiceRoutine* isrs[I86_IDT_MAX_ENTRY_COUNT];
    };
}

#endif
