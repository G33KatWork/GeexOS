#ifndef _ARCH_ACPI_AMLSDT_H
#define _ARCH_ACPI_AMLSDT_H

#include <lib/types.h>
#include <arch/ACPI/ACPITable.h>

namespace Arch
{
    namespace ACPI
    {
        struct AmlSDTDescriptor
        {
            struct ACPITableHeader h;
            char AMLCode[1];
            
        } __attribute__((packed));
        
        class AmlSDT : public ACPITable
        {
        private:
            struct AmlSDTDescriptor* descriptor;
            AmlSDT(Address a) { descriptor = (struct AmlSDTDescriptor*)a; }
        
        public:
            bool IsValid()
            {
                return ChecksumValid((Address)descriptor, descriptor->h.Length);
            }
            
            static AmlSDT* FromAddress(Address a)
            {
                return new AmlSDT(a);
            }
            
            size_t GetAMLCodeLength() { return descriptor->h.Length - sizeof(struct ACPITableHeader); }
            char* GetAMLCode() { return descriptor->AMLCode; }
        };
    }
}

#endif
