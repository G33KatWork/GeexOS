#ifndef _ARCH_ACPI_AMLSDT_H
#define _ARCH_ACPI_AMLSDT_H

#include <types.h>
#include <arch/internal/ACPI/ACPITable.h>

namespace Arch
{
    namespace ACPI
    {
		class AmlSDT : public ACPITable
        {
        public:
            AmlSDT(Address a)
				: ACPITable(a)
			{}
        
        public:
			size_t GetAMLCodeLength() { return GetLengthInternal() - ACPITABLEHEADERLEN; }
			char* GetAMLCode() { return Read(36, GetAMLCodeLength()); };
        };
    }
}

#endif
