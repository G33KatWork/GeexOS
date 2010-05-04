#ifndef _ARCH_ACPI_RSDT_H
#define _ARCH_ACPI_RSDT_H

#include <lib/types.h>
#include <arch/ACPI/RSDT.h>
#include <arch/ACPI/ACPITable.h>

//FIXME: Dirty casting hell... Perhaps we can use templates here to solve this

namespace Arch
{
    namespace ACPI
    {
        struct RSDTDescriptor
        {
            struct ACPITableHeader h;
            uint32_t NextSDT[1];
        } __attribute__((packed));
        
        class RSDT : public ACPITable
        {
        protected:
            Address descriptor;
            RSDT(Address a) { descriptor = a; }
        
        public:
            virtual unsigned int GetSubtableCount()
            {
                return (((struct RSDTDescriptor*)descriptor)->h.Length - sizeof(struct ACPITableHeader)) / 4;
            }
            
            virtual Address GetTable(const char* Signature);
            virtual Address GetTable(unsigned int index);
            
            virtual bool IsValid()
            {
                return ChecksumValid(descriptor, ((struct RSDTDescriptor*)descriptor)->h.Length);
            }
            
            static RSDT* FromAddress(Address a)
            {
                if(a == NULL) return NULL;
                return new RSDT(a);
            }
        };
        
        struct XSDTDescriptor
        {
            struct ACPITableHeader h;
            uint64_t NextSDT[1];
        } __attribute__((packed));
        
        class XSDT : public RSDT
        {
        private:
            XSDT(Address a)
                : RSDT(a)
            {}
        
        public:
            unsigned int GetSubtableCount()
            {
                return (((struct XSDTDescriptor*)descriptor)->h.Length - sizeof(struct ACPITableHeader)) / 8;
            }
            
            Address GetTable(const char* Signature);
            Address GetTable(unsigned int index);
            
            bool IsValid()
            {
                return ChecksumValid((Address)descriptor, ((struct RSDTDescriptor*)descriptor)->h.Length);
            }
            
            static XSDT* FromAddress(Address a)
            {
                if(a == NULL) return NULL;
                return new XSDT(a);
            }
        };
    }
}

#endif
