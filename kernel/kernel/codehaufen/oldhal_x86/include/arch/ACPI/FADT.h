#ifndef _ARCH_ACPI_FADT_H
#define _ARCH_ACPI_FADT_H

#include <types.h>
#include <arch/ACPI/ACPITable.h>
#include <arch/ACPI/GAS.h>

namespace Arch
{
    namespace ACPI
    {
		enum FADTFeatureFlags
		{
			WBINVD					=		(1<<0),
			WBINBVD_FLUSH			=		(1<<1),
			PROC_C1					=		(1<<2),
			P_LVL2_UP				=		(1<<3),
			PWR_BUTTON				=		(1<<4),
			SLP_BUTTON				=		(1<<5),
			FIX_RTC					=		(1<<6),
			RTC_S4					=		(1<<7),
			TMR_VAL_EXT				=		(1<<8),
			DCK_CAP					=		(1<<9),
			RESET_REG_SUP			=		(1<<10),
			SEALED_CASE				=		(1<<11),
			HEADLESS				=		(1<<12),
			CPU_SW_SLP				=		(1<<13),
			PCI_EXP_WAK				=		(1<<14),
			USE_PLATFORM_CLOCK		=		(1<<15),
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
		};
		
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
			
			uint8_t GetPreferredPmProfile() { return Read8(45); }
			uint16_t GetSCI_INT() { return Read16(46); }
			uint32_t GetSMI_CMD() { return Read32(48); }
			uint8_t GetACPIEnable() { return Read8(52); }
			uint8_t GetACPIDisable() { return Read8(53); }
			uint8_t GetS4BIOSRequest() { return Read8(54); }
			uint8_t GetPStateCount() { return Read8(55); }
			uint32_t GetPM1aEventBlock() { return Read32(56); }
			uint32_t GetPM1bEventBlock() { return Read32(60); }
			uint32_t GetPM1aCountBlock() { return Read32(64); }
			uint32_t GetPM1bCountBlock() { return Read32(68); }
			uint32_t GetPM2CountBlock() { return Read32(72); }
			uint32_t GetPMTimerBlock() { return Read32(76); }
			uint32_t GetGPE0Block() { return Read32(80); }
			uint32_t GetGPE1Block() { return Read32(84); }
			uint8_t GetPM1EventLength() { return Read8(88); }
			uint8_t GetPM1CountLength() { return Read8(89); }
			uint8_t GetPM2CountLength() { return Read8(90); }
			uint8_t GetPMTimerLength() { return Read8(91); }
			uint8_t GetGPE0BlockLength() { return Read8(92); }
			uint8_t GetGPE1BlockLength() { return Read8(93); }
			uint8_t GetGPE1Base() { return Read8(94); }
			uint8_t GetCSTCount() { return Read8(95); }
			uint16_t GetPLvl2Latency() { return Read16(96); }
			uint16_t GetPLvl3Latency() { return Read16(98); }
			uint16_t GetFlushSize() { return Read16(100); }
			uint16_t GetFlushStride() { return Read16(102); }
			uint8_t GetDutyOffset() { return Read8(104); }
			uint8_t GetDutyWidth() { return Read8(105); }
			uint8_t GetDayAlarm() { return Read8(106); }
			uint8_t GetMonthAlarm() { return Read8(107); }
			uint8_t GetCentury() { return Read8(108); }
			uint16_t GetIAPCBootArch() { return Read16(109); }
			uint32_t GetFlags() { return Read32(112); }
			GAS* GetResetRegister() { return (GAS*)Read(116, 12); }
			uint8_t GetResetValue() { return Read8(128); }
			GAS* GetX_PM1aEventBlock() { return (GAS*)Read(148, 12); }
			GAS* GetX_PM1bEventBlock() { return (GAS*)Read(160, 12); }
			GAS* GetX_PM1aCountBlock() { return (GAS*)Read(172, 12); }
			GAS* GetX_PM1bCountBlock() { return (GAS*)Read(184, 12); }
			GAS* GetX_PM2CountBlock() { return (GAS*)Read(196, 12); }
			GAS* GetX_PMTimerBlock() { return (GAS*)Read(208, 12); }
			GAS* GetX_GPE0Block() { return (GAS*)Read(220, 12); }
			GAS* GetX_GPE1Block() { return (GAS*)Read(232, 12); }
        };
    }
}

#endif
