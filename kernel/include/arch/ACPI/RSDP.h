#ifndef _ARCH_ACPI_RSDP_H
#define _ARCH_ACPI_RSDP_H

#include <lib/types.h>

namespace Arch
{
    namespace ACPI
    {
        #define RSDPLENGTH_V1   20
        #define RSDPLENGTH_V2   36
        
        typedef struct
        {
            char Signature[8];
            char Checksum;
            char OEMID[6];
            char Revision;
            Address RsdtAddress;
            //--- Following fields only available in Version 2.0 and up
            uint32_t Length;
            unsigned long long XsdtAddress;
            char ExtendedChecksum;
            char reserved[3];
        } RSDPDescriptor;
        
        class RSDP
        {
        private:
            RSDPDescriptor* descriptor;
            
            bool Find(Address start, size_t size);
            bool ChecksumValid(Address start, size_t len);
            
        public:
            RSDP();
            
            bool Found() { return descriptor != NULL; }
            char* GetOEMID() { return descriptor->OEMID; }
            char GetRevision() { return descriptor->Revision; }
            
            bool IsValid();
            
            Address GetRSDTAddress() { return descriptor->RsdtAddress; }
            bool XSDTAvailable() { return descriptor->Revision > 0; }
            Address GetXSDTAddress() { return descriptor->XsdtAddress; }
        };
    }
}

#endif
