#ifndef _ARCH_HAL_H
#define _ARCH_HAL_H

#include <lib/types.h>
#include <arch/pit.h>
#include <arch/clock_source.h>

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
        
        //Set WP (Write Protect) bit in CR0
        //This causes a page fault when writing to read-only pages even if we are in kernel mode
        asm volatile (
    		"mov %cr0, %eax\n"
    		"orl $0x10000, %eax\n"
    		"mov %eax, %cr0\n");
    }
    
    extern ClockSource_t ClockSource;
}

#endif
