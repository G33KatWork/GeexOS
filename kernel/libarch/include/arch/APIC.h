#ifndef _ARCH_APIC_H
#define _ARCH_APIC_H

#include <types.h>

namespace Arch
{
    class APIC
    {
    private:
        uint32_t readIOApic(Address addr, uint32_t reg);
        void writeIOApic(Address addr, uint32_t reg, uint32_t val);
        
    public:
        void SetBaseAddress(Address newBase);
        Address GetBaseAddress(void);
    };
    
    bool CPUsupportsAPIC(void);
}

#endif
