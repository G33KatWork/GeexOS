#ifndef _ARCH_ACPI_FADT_H
#define _ARCH_ACPI_FADT_H

#include <lib/types.h>
#include <arch/ACPI/ACPITable.h>

namespace Arch
{
    namespace ACPI
    {
		/*enum FADTFeatureFlags
		{
			WBINVD					=		(1<<0),
			WBINBVD_FLUSH			=		(1<<1),
			PROC_C1					=		(1<<2),
			P_LVL2_UP				=		(1<<3),
			PWR_BUTTON				=		(1<<4),
			SLP_BUTTON				=		(1<<5),
			FIX_RTC					=		(1<<6),
			RTC_S4					=		(1<<7),
			TMR_V AL_EXT			=		(1<<8),
			DCK_CAP					=		(1<<9),
			RESET_REG_SUP			=		(1<<10),
			SEALED_CASE				=		(1<<11),
			HEADLESS				=		(1<<12),
			CPU_SW_SLP				=		(1<<13),
			PCI_EXP_W AK			=		(1<<14),
			USE_PLA TFORM_CLOCK		=		(1<<15),
			S4_RTC_STS_VALID		=		(1<<16),
			REMOTE_POWER_ON_CAPABLE	=		(1<<17),
			FORCE_APIC_CLUSTER_MODEL=		(1<<18),
			FORCE_APIC_PHYSICAL_DESTINATION_MODE	=	(1<<19)	
		};
		
		enum FADTPrefferedPMProfile
		{
			Unspecified				=		0,
			Desktop					=		1,
			Mobile					=		2,
			Workstation				=		3,
			EnterpriseServer		=		4,
			SOHOServer				=		5,
			AppliancePC				=		6,
			PerformanceServer		=		7
		};*/
		
		class FADT : public ACPITable
        {
        public:
            FADT(Address a)
				: ACPITable(a)
			{}
        
        public:
			Address GetFACSAddress()
			{
				if(Read32(36) != NULL)
					return TranslatePhysical(Read32(36));
				else
					return TranslatePhysical(Read64(132));
			}
            
			Address GetDSDTAddress()
			{
				if(Read32(40) != NULL)
					return TranslatePhysical(Read32(40));
				else
					return TranslatePhysical(Read64(140));
			}
        };

        /*struct FADTDescriptor
        {
            struct ACPITableHeader h;
            Address FACSAddr;
            Address DSDTAddr;
            char reserved;
			char PreferredPmProfile;
			short SCI_INT;
			uint32_t SMI_CMD;
			char ACPIEnable;
			char ACPIDisable;
			char S4BIOSRequest;
			char PStateCount;
			uint32_t PM1aEventBlock;
			uint32_t PM1bEventBlock;
			uint32_t PM1aCountBlock;
			uint32_t PM1bCountBlock;
			uint32_t PM2CountBlock;
			uint32_t PMTimerBlock;
			uint32_t GPE0Block;
			uint32_t GPE1Block;
			char PM1EventLength;
			char PM1CountLength;
			char PM2CountLength;
			char PMTimerLength;
			char GPE0BlockLength;
			char GPE1BlockLength;
			char GPE1Base;
			char CSTCount;
			short PLvl2Latency;
			short PLvl3Latency;
			short FlushSize;
			short FlushStride;
			char DutyOffset;
			char DutyWidth;
			char DayAlarm;
			char MonthAlarm;
			char Century;
			short IAPCBootArch;
			char reserved2;
			uint32_t flags;
			uint96_t ResetRegister;
			char ResetValue;
			char reserved3[3];
			uint64_t FACSAddr64;
			uint64_t DSDTAddr64;
			uint96_t X_PM1aEventBlock;
			uint96_t X_PM1bEventBlock;
			uint96_t X_PM1aCountBlock;
			uint96_t X_PM1bCountBlock;
			uint96_t X_PM2CountBlock;
			uint96_t X_PMTimerBlock;
			uint96_t X_GPE0Block;
			uint96_t X_GPE1Block;
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
                if(a == NULL) return NULL;
                return new FADT(a);
            }
            
            
        };*/
    }
}

#endif
