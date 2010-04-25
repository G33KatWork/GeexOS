#ifndef _ARCH_ACPI_RSDT_H
#define _ARCH_ACPI_RSDT_H

#include <lib/types.h>
#include <arch/ACPI/RSDT.h>
#include <arch/ACPI/ACPITable.h>

namespace Arch
{
    namespace ACPI
    {
        struct RSDTDescriptor
        {
            struct ACPITableHeader h;
            Address NextSDT[1];
        } __attribute__((packed));
        
        class RSDT : public ACPITable
        {
        private:
            struct RSDTDescriptor* descriptor;
            RSDT(Address a) { descriptor = (struct RSDTDescriptor*)a; }
        
        public:
            unsigned int GetSubtableCount()
            {
                return (descriptor->h.Length - sizeof(struct ACPITableHeader)) / 4;
            }
            
            ACPITableHeader* GetTable(const char* Signature);
            Address GetTable(unsigned int index);
            
            bool IsValid()
            {
                return ChecksumValid((Address)descriptor, descriptor->h.Length);
            }
            
            static RSDT* FromAddress(Address a)
            {
                return new RSDT(a);
            }
        };
    }
}

#endif
