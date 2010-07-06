#ifndef _ARCH_HAL_H
#define _ARCH_HAL_H

#include <types.h>
#include <arch/pit.h>
#include <arch/clock_source.h>
#include <kernel/utils/Multiboot.h>

namespace Arch
{
    void InitializeCPU();
    void InitDone();
    void GetCPUVendor(char* buf);
    
    inline void HaltMachine()
    {
        asm volatile ("cli");
        asm volatile ("hlt");
    }
    
    inline void InitializeTimer()
    {
        PIT::GetInstance()->Initialize(1000); //1000Hz
    }
    
    void SetupArchMemRegions(Multiboot* m);
    
    extern ClockSource_t ClockSource;
}

#endif
