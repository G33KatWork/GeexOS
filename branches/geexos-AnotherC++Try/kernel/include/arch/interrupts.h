#ifndef _ARCH_INTERRUPTS_H
#define _ARCH_INTERRUPTS_H

#include <lib/types.h>

namespace Arch
{

    inline void EnableInterrupts()
    {
        asm volatile ("sti");
    }

    inline void DisableInterrupts()
    {
        asm volatile ("cli");
    }
    
    inline void HaltMachine()
    {
        asm volatile ("hlt");
    }
}

#endif
