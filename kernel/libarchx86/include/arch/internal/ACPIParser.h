#ifndef _ARCHX86_ACPIPARSER_H
#define _ARCHX86_ACPIPARSER_H

#include <types.h>
#include <arch/types.h>
#include <arch/internal/ACPI/RSDT.h>
#include <arch/internal/ACPI/FADT.h>
#include <arch/internal/ACPI/AmlSDT.h>
#include <arch/internal/ACPI/HPET.h>
#include <arch/internal/ACPI/MADT.h>

namespace Arch
{   
    class ACPIParser
    {
    public:
        ACPIParser()
        {
            rsdt = NULL;
            fadt = NULL;
            dsdt = NULL;
            ssdt = NULL;
            hpet = NULL;
            madt = NULL;
        }
    
        void Parse();
        
        ACPI::FADT* GetFADT() { return fadt; }
        ACPI::AmlSDT* GetDSDT() { return dsdt; }
        ACPI::AmlSDT* GetSSDT() { return ssdt; }
        ACPI::HPET* GetHPET() { return hpet; }
        ACPI::MADT* GetMADT() { return madt; }
    
    private:
        ACPI::RSDT* rsdt;
        
        ACPI::FADT* fadt;
        ACPI::AmlSDT* dsdt;
        ACPI::AmlSDT* ssdt;
        ACPI::HPET* hpet;
        ACPI::MADT* madt;
    };
}

#endif
