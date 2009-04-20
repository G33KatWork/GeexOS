#ifndef _ARCH_PIT_H
#define _ARCH_PIT_H

#include <lib/types.h>
#include <kernel/IInterruptServiceRoutine.h>

using namespace Kernel;

namespace Arch
{
    class PIT : public IInterruptServiceRoutine
    {
    private:
        static PIT* instance;
        long ticks;
        
        PIT();
        
    public:
        static PIT* GetInstance();
        
        void Initialize(uint32_t frequency);
        long GetTickCount();
        void Execute(registers_t regs);
    };
}

#endif
