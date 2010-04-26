#ifndef _ARCH_ACPI_HPET_H
#define _ARCH_ACPI_HPET_H

#include <lib/types.h>
#include <arch/ACPI/ACPITable.h>

namespace Arch
{
    namespace ACPI
    {
        struct HPETDescriptor
        {
            struct ACPITableHeader h;
            uint32_t BlockID;
            uint32_t BaseAddress;   //This is not a linear address, refer to HPEC spec from intel
            char HPETNumber;
            short MainCounterMinimumPeriodicClockTick;
            char PageProtectionOEMAttribute;
        } __attribute__((packed));
        
        class HPET : public ACPITable
        {
        private:
            struct HPETDescriptor* descriptor;
            HPET(Address a) { descriptor = (struct HPETDescriptor*)a; }
        
        public:
            bool IsValid()
            {
                return ChecksumValid((Address)descriptor, descriptor->h.Length);
            }
            
            static HPET* FromAddress(Address a)
            {
                return new HPET(a);
            }
        };
    }
}

#endif
