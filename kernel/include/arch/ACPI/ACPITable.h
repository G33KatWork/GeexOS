#ifndef _ARCH_ACPI_TABLE_H
#define _ARCH_ACPI_TABLE_H

#include <lib/types.h>

namespace Arch
{
    namespace ACPI
    {
        struct ACPITableHeader
        {
            char Signature[4];
            uint32_t Length;
            char Revision;
            char Checksum;
            char OEMID[6];
            char OEMTableID[8];
            uint32_t OEMRevision;
            uint32_t CreatorID;
            uint32_t CreatorRevision;
        } __attribute__((packed));
        
        class ACPITable
        {
        protected:
            bool ChecksumValid(Address start, size_t len);
        };
    }
}

#endif
