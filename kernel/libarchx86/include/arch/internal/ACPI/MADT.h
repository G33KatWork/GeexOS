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
			MADT_LOCALAPIC					=		0,
			MADT_IOAPIC						=		1,
			MADT_INTERRUPTSOURCEOVERRIDE	=		2,
			MADT_NMISOURCE					=		3,
			MADT_LOCALAPICNMI				=		4,
			MADT_LOCALAPICADDRESSOVERRIDE	=		5,
			MADT_IOSAPIC					=		6,
			MADT_LOCALSAPIC					=		7,
			MADT_PLATFORMINTERRUPTSOURCES	=		8,
			MADT_LOCALX2APIC				=		9,
			MADT_LOCALX2APICNMI				=		0xA
		};
		
		enum LocalAPICStructureFlags
		{
			LAPIC_ENABLED			=		(1<<0),
			LAPIC_STRUCT_RESERVED	=		0xFFFFFFFE
		};
		
		enum InterruptSourceOverrideStructureFlags
		{
			POLARITY_BUSSPEC			=	0,  //0000
			POLARITY_ACTIVEHIGH			=	1,  //0001
			POLARITY_RESERVED			=	2,  //0010
			POLARITY_ACTIVE				=	3,  //0011
			
			TRIGGER_BUSSPEC				=	0,  //0000
			TRIGERMODE_EDGE				=	4,  //0100
			TRIGGERMODE_RESERVED		=	8,  //1000
			TRIGGERMODE_LEVEL			=	12  //1100
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
			
			/**
			 * Returns the whole amount of subsctructures inside the MADT
			**/
            unsigned int GetAPICStructureCount();
			
			/**
			 * Returns the substructure and type of it at a given index
			 * If the index is out of bounds, NULL is returned.
			 * type may be a NULL pointer if not needed
			 *
			 * \param n the index
			 * \param type a pointer to a ApicStructureTypes variable which will be set to the type of the found substruct
			 * \return the found substructure or NULL if the given index was out of bounds
			**/
            APICStructureHeader* GetAPICStruct(unsigned int n, ApicStructureTypes* type);
			
			/**
			 * Returns the first substructure of a given type beginning at index n
			 * 
			 * \param start the offset to begin the search at
			 * \param type the desired type of the next structure to be found
			 * \return The next found substructure or NULL
			**/
            APICStructureHeader* GetAPICStructOfType(unsigned int start, ApicStructureTypes type);
			
			/**
			 * Returns the index of a given substructure pointer or -1 if it is not found
			 *
			 * \param apicStruct the substructure the index of should be determined
			 * \return the index of the given struct or -1 if not found
			**/
            int GetAPICStructIndex(APICStructureHeader* apicStruct);
			
			/**
			 * Returns the next substructure of a given type after a given previous substructure
			 * 
			 * \param previous the pointer to the previous structure
			 * \type the desired type of the next structure to be found
			 * \return the next found structure or NULL
			**/
            APICStructureHeader* GetNextAPICStructOfType(APICStructureHeader* previous, ApicStructureTypes type);
        };
    }
}

#endif
