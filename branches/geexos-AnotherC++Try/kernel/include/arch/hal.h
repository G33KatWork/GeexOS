#ifndef _ARCH_HAL_H
#define _ARCH_HAL_H

#include <lib/types.h>

namespace Arch
{
    void InitializeCPU();
    
    inline void HaltMachine()
    {
        asm volatile ("hlt");
    }
}

#endif
