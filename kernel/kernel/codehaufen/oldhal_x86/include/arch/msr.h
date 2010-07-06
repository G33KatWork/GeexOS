#ifndef _ARCH_MSR_H
#define _ARCH_MSR_H

#include <arch/cpuid.h>

#define MSR_APIC_BASE           0x1B

namespace Arch
{
    static inline bool CPUhasMSR(void)
    {
        uint32_t a, d;
        cpuid(CPUID_GETFEATURES, &a, &d);
        return d & CPUID_FLAG_MSR;
    }

    static inline void getMSR(uint32_t msr, uint32_t* lo, uint32_t* hi)
    {
       asm volatile("rdmsr":"=a"(*lo),"=d"(*hi):"c"(msr));
    }

    static inline void setMSR(uint32_t msr, uint32_t lo, uint32_t hi)
    {
       asm volatile("wrmsr"::"a"(lo),"d"(hi),"c"(msr));
    }

}

#endif
