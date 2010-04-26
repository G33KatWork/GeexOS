#ifndef _ARCH_ACPI_FADT_H
#define _ARCH_ACPI_FADT_H

#include <lib/types.h>
#include <arch/ACPI/ACPITable.h>

namespace Arch
{
    namespace ACPI
    {
        struct FADTDescriptor
        {
            struct ACPITableHeader h;
            Address FACSAddr;
            Address DSDTAddr;
            char reserved;
            
        } __attribute__((packed));
        
        class FADT : public ACPITable
        {
        private:
            struct FADTDescriptor* descriptor;
            FADT(Address a) { descriptor = (struct FADTDescriptor*)a; }
        
        public:
            bool IsValid()
            {
                return ChecksumValid((Address)descriptor, descriptor->h.Length);
            }
            
            static FADT* FromAddress(Address a)
            {
                return new FADT(a);
            }
            
            Address GetFACSAddress() { return descriptor->FACSAddr; }
            Address GetDSDTAddress() { return descriptor->DSDTAddr; }
        };
    }
}

#endif
