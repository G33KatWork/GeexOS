#ifndef _ARCH_HAL_H
#define _ARCH_HAL_H

#include <lib/types.h>
#include <arch/pit.h>

namespace Arch
{
    void InitializeCPU();
    
    inline void HaltMachine()
    {
        asm volatile ("hlt");
    }
    
    inline long GetTickCount()
    {
        return PIT::GetInstance()->GetTickCount();
    }
    
    inline void InitializeTimer(int frequency)
    {
        PIT::GetInstance()->Initialize(frequency);
    }
}

#endif
