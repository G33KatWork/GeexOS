#ifndef _ARCH_ACPI_MADT_H
#define _ARCH_ACPI_MADT_H

#include <types.h>
#include <arch/internal/ACPI/ACPITable.h>

namespace Arch
{
    namespace ACPI
    {
		enum ApicFlags
		{
			PCAT_COMPAT				=		(1<<0),
			ACPI_FLAGS_RESERVED		=		0xFFFFFFFE
		};
		
		enum ApicStructureTypes
		{
			LOCALAPIC					=		0,
			IOAPIC						=		1,
			INTERRUPTSOURCEOVERRIDE		=		2,
			NMISOURCE					=		3,
			LOCALAPICNMI				=		4,
			LOCALAPICADDRESSOVERRIDE	=		5,
			IOSAPIC						=		6,
			LOCALSAPIC					=		7,
			PLATFORMINTERRUPTSOURCES	=		8,
			LOCALX2APIC					=		9,
			LOCALX2APICNMI				=		0xA
		};
		
		enum LocalAPICStructureFlags
		{
			LAPIC_ENABLED			=		(1<<0),
			LAPIC_STRUCT_RESERVED	=		0xFFFFFFFE
		};
		
		enum InterruptSourceOverrideStructureFlags
		{
			POLARITY_BUSSPEC			=	0,
			POLARITY_ACTIVEHIGH			=	1,
			POLARITY_RESERVED			=	2,
			POLARITY_ACTIVE				=	3,
			TRIGGER_BUSSPEC				=	0,
			TRIGERMODE_EDGE				=	1,
			TRIGGERMODE_RESERVED		=	2,
			TRIGGERMODE_LEVEL			=	3
		};
		
		enum PlatformInterruptSourceFlags
		{
			PLATFORMINTERRUPTSOURCE_CPEIPROCESSOROVERRIDE	=	(1<<0),
			PLATFORMINTERRUPTSOURCE_RESERVED				=	0xFFFFFFFE
		};
		
		typedef struct APICStructureHeader_
		{
			uint8_t Type;
			uint8_t Length;
		} APICStructureHeader;
		
		typedef struct LocalAPICStructure_
		{
			APICStructureHeader APICHeader;
			uint8_t ProcessorID;
			uint8_t APICID;
			uint32_t Flags;
		} LocalAPICStructure;
		
		typedef struct IOAPICStructure_
		{
			APICStructureHeader APICHeader;
			uint8_t IOAPICID;
			uint8_t reserved;
			uint32_t IOAPICAddress;
			uint32_t GlobalSystemInterruptBase;
		} IOAPICStructure;
		
		typedef struct InterruptSourceOverrideStructure_
		{
			APICStructureHeader APICHeader;
			uint8_t Bus;
			uint8_t Source;
			uint32_t GlobalSystemInterrupt;
			uint16_t Flags;
		} InterruptSourceOverrideStructure;
		
		typedef struct NMIInterruptSourceStructure_
		{
			APICStructureHeader APICHeader;
			uint16_t Flags;
			uint32_t GlobalSystemInterrupt;
		} NMIInterruptSourceStructure;
		
		typedef struct LocalAPICNMIStructure_
		{
			APICStructureHeader APICHeader;
			uint8_t ProcessorID;
			uint16_t Flags;
			uint8_t LocalAPICLINT;
		} LocalAPICNMIStructure;
		
		typedef struct LocalAPICAddressOverrideStructure_
		{
			APICStructureHeader APICHeader;
			uint16_t reserved;
			uint64_t LocalAPICAddress;
		} LocalAPICAddressOverrideStructure;
		
		typedef struct IOSAPICStructure_
		{
			APICStructureHeader APICHeader;
			uint8_t IOAPICID;
			uint8_t reserved;
			uint32_t GlobalSystemInterruptBase;
			uint64_t IOSAPICAddress;
		} IOSAPICStructure;
		
		typedef struct LocalSAPICStructure_
		{
			APICStructureHeader APICHeader;
			uint8_t ProcessorID;
			uint8_t LocalSAPICID;
			uint8_t LocalSAPICEID;
			uint16_t reserved1;
			uint8_t reserved2;
			uint32_t ProcessorUIDValue;
			const char* ProcessorUIDString;
		} LocalSAPICStructure;
		
		typedef struct PlatformInterruptSourceStructure_
		{
			APICStructureHeader APICHeader;
			uint16_t Flags;
			uint8_t InterruptType;
			uint8_t ProcessorEID;
			uint8_t IOSAPICVector;
			uint32_t GlobalSystemInterrupt;
			uint32_t PlatformInterruptSourceFlags;
		} PlatformInterruptSourceStructure;
		
		typedef struct ProcessorLocalX2APICStructure_
		{
			APICStructureHeader APICHeader;
			uint16_t reserved;
			uint32_t X2APICID;
			uint32_t Flags;
			uint32_t ACPIProcessorUID;
		} ProcessorLocalX2APICStructure;
		
		typedef struct LocalX2APICNMIStructure_
		{
			APICStructureHeader APICHeader;
			uint16_t Flags;
			uint32_t ACPIProcessorUID;
			uint8_t LocalX2APICLINT;
			uint16_t reserved1;
			uint8_t reserved2;
		} LocalX2APICNMIStructure;
		
		class MADT : public ACPITable
        {
        public:
            MADT(Address a)
				: ACPITable(a)
			{}
        
        public:
			uint32_t GetLocalAPICAddress() { return Read32(36); }
			uint32_t GetFlags() { return Read32(40); }
			size_t GetAPICStructureCount()
			{
				size_t baseSize = ACPITABLEHEADERLEN + 2*sizeof(uint32_t);
				size_t curSize = baseSize;
				size_t structCount = 0;
				Address start = GetAddress();
				
				while(curSize < GetLengthInternal())
				{
					structCount++;
					APICStructureHeader* struc = (APICStructureHeader*)(start + curSize);
					curSize += struc->Length;
				}
				
				return structCount;
			}
			
			APICStructureHeader* GetAPICStruct(size_t n, uint8_t* type)
			{
				size_t baseSize = ACPITABLEHEADERLEN + 2*sizeof(uint32_t);
				size_t curSize = baseSize;
				Address start = GetAddress();
				size_t i = 0;
				
				while(i < n)
				{
					ASSERT(curSize < GetLengthInternal(), "Index out of bounds");
					i++;
					APICStructureHeader* struc = (APICStructureHeader*)(start + curSize);
					curSize += struc->Length;
				}
				
				APICStructureHeader* retStruct = (APICStructureHeader*)(GetAddress() + curSize);
				if(type != NULL)
					*type = retStruct->Type;
				return retStruct;
			}
        };
    }
}

#endif
