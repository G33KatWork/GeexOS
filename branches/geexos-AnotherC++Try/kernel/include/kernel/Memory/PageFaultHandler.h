#ifndef PAGE_FAULT_HANDLER_H_
#define PAGE_FAULT_HANDLER_H_

#include <lib/types.h>
#include <kernel/IInterruptServiceRoutine.h>

using namespace Kernel;

namespace Memory
{
    class PageFaultHandler : public IInterruptServiceRoutine
    {
    public:
        void Execute(registers_t regs);
    };
}

#endif
