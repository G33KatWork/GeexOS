#ifndef _ARCH_INTERRUPTS_H
#define _ARCH_INTERRUPTS_H

#include <lib/types.h>

namespace Arch
{
    void InitializeInterrupts();
    
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
