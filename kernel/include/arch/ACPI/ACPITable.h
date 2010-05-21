#ifndef _ARCH_ACPI_TABLE_H
#define _ARCH_ACPI_TABLE_H

#include <lib/types.h>
#include <kernel/global.h>
#include <kernel/debug.h>

namespace Arch
{
    namespace ACPI
    {
        /*struct ACPITableHeader
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
        } __attribute__((packed));*/
        
		#define ACPITABLEHEADERLEN	36

        class ACPITable
        {
		private:
			Address tableStart;
		
		protected:	
			size_t GetLengthInternal() { return ((size_t*)tableStart)[1]; }
		
			char* Read(size_t offset, size_t length);
			uint8_t Read8(size_t offset) { return *(uint8_t*)Read(offset, sizeof(uint8_t)); }
			uint16_t Read16(size_t offset)	{ return *(uint16_t*)Read(offset, sizeof(uint16_t)); }
			uint32_t Read32(size_t offset)	{ return *(uint32_t*)Read(offset, sizeof(uint32_t)); }
			uint64_t Read64(size_t offset)	{ return *(uint64_t*)Read(offset, sizeof(uint64_t)); }
			//Gas Read(size_t offset);
			
            bool ChecksumValid(Address start, size_t len);
            Address TranslatePhysical(Address physical);

		public:
			ACPITable(Address start) { tableStart = start; }
			
			Address GetAddress() { return tableStart; }
            char* Signature() { char* foo = Read(0, 4); return foo; }
            uint32_t Length() { return Read32(4); }
			uint8_t Revision() { return Read8(8); }
			uint8_t Checksum() { return Read8(9); }
			char* OEMID() { return Read(10, 6); }
			char* OEMTableID() { return Read(16, 8); }
			uint32_t OEMRevision() { return Read32(24); }
			uint32_t OEMCreatorID() { return Read32(28); }
			uint32_t CreatorRevision() { return Read32(32); }
			
			bool IsValid() { return ChecksumValid((Address)tableStart, GetLengthInternal()); }
			
			template<class T>
			static T* FromAddress(Address a)
			{
				if(a == NULL) return NULL;
                return new T(a);
			}
        };
    }
}

#endif
