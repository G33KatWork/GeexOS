#ifndef _ARCH_ACPI_RSDT_H
#define _ARCH_ACPI_RSDT_H

#include <types.h>
#include <arch/ACPI/RSDT.h>
#include <arch/ACPI/ACPITable.h>

//FIXME: Dirty casting hell... Perhaps we can use templates here to solve this

namespace Arch
{
    namespace ACPI
    {
        /*struct RSDTDescriptor
        {
            struct ACPITableHeader h;
            uint32_t NextSDT[1];
        } __attribute__((packed));*/
        
        class RSDT : public ACPITable
        {
        public:
            RSDT(Address a)
				: ACPITable(a)
			{}
        
        public:
            virtual unsigned int GetSubtableCount() { return (Length() - ACPITABLEHEADERLEN) / 4; }
            
            virtual Address GetTable(const char* SubtableSignature);
            virtual Address GetTable(unsigned int index);
        };
        
        /*struct XSDTDescriptor
        {
            struct ACPITableHeader h;
            uint64_t NextSDT[1];
        } __attribute__((packed));*/
        
        class XSDT : public RSDT
        {
        public:
            XSDT(Address a)
                : RSDT(a)
            {}
        
        public:
            unsigned int GetSubtableCount() { return (Length() - ACPITABLEHEADERLEN) / 8; }
            
            Address GetTable(const char* SubtableSignature);
            Address GetTable(unsigned int index);
        };
    }
}

#endif
