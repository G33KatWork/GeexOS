#ifndef _ARCH_HAL_H
#define _ARCH_HAL_H

#include <lib/types.h>
#include <arch/pit.h>
#include <arch/clock_source.h>

#define     BIOS_ADDRESS        0x0
#define     BIOS_SIZE           0x10000

namespace Arch
{
    void InitializeCPU();
    
    inline void HaltMachine()
    {
        asm volatile ("cli");
        asm volatile ("hlt");
    }
    
    inline void InitializeTimer()
    {
        PIT::GetInstance()->Initialize(1000); //1000Hz
    }
    
    void SetupArchMemRegions(void);
    
    extern ClockSource_t ClockSource;
}

#endif
