#ifndef _ARCH_ACPI_HPET_H
#define _ARCH_ACPI_HPET_H

#include <lib/types.h>
#include <arch/ACPI/ACPITable.h>

namespace Arch
{
    namespace ACPI
    {
		class HPET : public ACPITable
        {
        public:
            HPET(Address a)
				: ACPITable(a)
			{}
        
        public:
			uint32_t GetBlockID() { return Read32(36); }
			uint32_t GetBaseAddress() { return Read32(40); }
			uint8_t GetHPETNumber() { return Read8(44); }
			uint16_t GetMainCounterMinimumPeriodicClock() { return Read16(45); }
			uint8_t GetPageProtectionOEMAttribute() { return Read8(47); }
        };
    }
}

#endif
