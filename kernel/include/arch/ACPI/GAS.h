#ifndef _ARCH_ACPI_GAS_H
#define _ARCH_ACPI_GAS_H

#include <lib/types.h>

namespace Arch
{
    namespace ACPI
    {
		enum GAS_AddressSpaceID
		{
			SystemMemory 			=	0,
			SystemIO				=	1,
			PCIConfigSpace			=	2,
			EmbeddedController		=	3,
			SMBus					=	4,
			FunctionFixedHardware	=	0x7F
		};
		
		enum GAS_AccessSize
		{
			Undefined				=	0,
			ByteAccess				=	1,
			WordAccess				=	2,
			DwordAccess				=	3,
			QWordAccess				=	4
		};
		
		typedef struct GAS_
		{
			uint8_t AddressSpaceID;
			uint8_t RegisterBitWidth;
			uint8_t RegisterBitOffset;
			uint8_t AccessSize;
			uint64_t Address;
		 } GAS;
    }
}

#endif
