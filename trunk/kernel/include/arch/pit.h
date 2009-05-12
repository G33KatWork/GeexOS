#ifndef _ARCH_PIT_H
#define _ARCH_PIT_H

#include <lib/types.h>
#include <kernel/IInterruptServiceRoutine.h>
#include <arch/clock_source.h>
#include <arch/interrupts.h>

using namespace Kernel;

namespace Arch
{
    class PIT
    {
    private:
        static PIT* instance;
        
        PIT();
        
    public:
        static void Disable() { MaskIRQ(IRQ_TIMER); }
        static void Enable() { UnmaskIRQ(IRQ_TIMER); }
        
        static PIT* GetInstance();
        
        void Initialize(uint32_t frequency);
    };
}

#endif
