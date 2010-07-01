#ifndef _ARCH_INTERRUPTS_H
#define _ARCH_INTERRUPTS_H

#include <types.h>
#include <kernel/IInterruptServiceRoutine.h>
#include <arch/idt.h>
#include <arch/pic.h>

// The following devices use PIC 1 to generate interrupts
#define		IRQ_TIMER			32
#define		IRQ_KEYBOARD		33
#define		IRQ_SERIAL2			34
#define		IRQ_SERIAL1			35
#define		IRQ_PARALLEL2		36
#define		IRQ_DISKETTE		37
#define		IRQ_PARALLEL1		38

// The following devices use PIC 2 to generate interrupts
#define		IRQ_CMOSTIMER		39
#define		IRQ_CGARETRACE		40
#define		IRQ_AUXILIARY		41
#define		IRQ_FPU				42
#define		IRQ_HDC				43

// Set the following number to the maximum nested exceptions
// the kernel tries to resolve before it will panic
#define     MAX_NESTED_EXCEPTIONS   1

using namespace Kernel;

//Handler called by our asm stubs
extern "C" void fault_handler(registers_t regs);
extern "C" void interrupt_handler(registers_t regs);

namespace Arch
{
    class InterruptDispatcher : public IInterruptServiceRoutine
    {
    private:
        static InterruptDispatcher* instance;

        IInterruptServiceRoutine* isrs[I86_IDT_MAX_ENTRY_COUNT];

        InterruptDispatcher();

    public:
        static InterruptDispatcher* GetInstance();

        void RegisterHandler(int i, IInterruptServiceRoutine* isr);
        void UnregisterHandler(int i);
        void Execute(registers_t *regs);
    };
    
    
    inline void EnableInterrupts()
    {
        asm volatile ("sti");
    }

    inline void DisableInterrupts()
    {
        asm volatile ("cli");
    }
}

#endif
