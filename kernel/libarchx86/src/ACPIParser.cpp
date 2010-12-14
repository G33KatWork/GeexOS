#include <arch/internal/ACPIParser.h>
#include <types.h>
#include <arch/internal/ACPI/RSDP.h>
#include <arch/internal/ACPI/RSDT.h>
#include <arch/internal/ACPI/FADT.h>
#include <arch/internal/ACPI/AmlSDT.h>
#include <arch/internal/ACPI/HPET.h>
#include <arch/internal/ACPI/MADT.h>

using namespace Arch;
using namespace Arch::ACPI;
using namespace Debug;

void ACPIParser::Parse()
{
    HAL_ACPI_DEBUG_MSG("Parsing ACPI...");
    RSDP* p = new RSDP();
    
    if(p->Found())
    {
        HAL_ACPI_DEBUG_MSG("ACPI seems to be available...");
        ASSERT(p->IsValid(), "RSDP is invalid");
        
        HAL_ACPI_DEBUG_MSG("ACPI OEM ID: " << p->GetOEMID());
        
        HAL_ACPI_DEBUG_MSG("RSDT Address: " << p->GetRSDTAddress());
        HAL_ACPI_DEBUG_MSG("XSDT Address: " << p->GetXSDTAddress());
        
        RSDT* rsdt;
        
        if(p->GetRevision() > 0 && p->GetXSDTAddress() != NULL)
        {
            HAL_ACPI_DEBUG_MSG("We have at least ACPI 2 and XSDT is available");
            rsdt = ACPITable::FromAddress<XSDT>(p->GetXSDTAddress());
        }
        else
        {
            HAL_ACPI_DEBUG_MSG("XSDT is not available, using RSDT");
            rsdt = ACPITable::FromAddress<RSDT>(p->GetRSDTAddress());
        }
    
        ASSERT(rsdt->IsValid(), "Checksum of RSDT/XSDT is invalid");
    
        HAL_ACPI_DEBUG_MSG("RSDT/XSDT contains " << dec << rsdt->GetSubtableCount() << " Subtables");
        for(unsigned int i = 0; i < rsdt->GetSubtableCount(); i++)
        {
            Address a = rsdt->GetTable(i);
            //kdbg.PrintData(a, 0x40);
            ACPITable table = ACPITable(a);
            HAL_ACPI_DEBUG_MSG("Address of Subtable " << dec << i << ": " << hex << a);
            //Signatures are not null-terminated, anyway we just print this here 
            HAL_ACPI_DEBUG_MSG("Subtable Signature: " << table.Signature());
            HAL_ACPI_DEBUG_MSG("Subtable OEMID: " << table.OEMID());
        }
    
        FADT* fadt = ACPITable::FromAddress<FADT>(rsdt->GetTable("FACP"));
        if(fadt != NULL)
        {
            HAL_ACPI_DEBUG_MSG("Found FADT");
            ASSERT(fadt->IsValid(), "FADT is invalid");
            HAL_ACPI_DEBUG_MSG("FACS Address: " << hex << fadt->GetFACSAddress());
            HAL_ACPI_DEBUG_MSG("DSDT Address: " << hex << fadt->GetDSDTAddress());
        
            if(fadt->GetFACSAddress() != NULL)
            {
                HAL_ACPI_DEBUG_MSG("Found FACS");
                //TODO: Write class for FACS
            }
        
            AmlSDT* dsdt = ACPITable::FromAddress<AmlSDT>(fadt->GetDSDTAddress());
            ASSERT(dsdt->IsValid(), "DSDT is invalid");
            HAL_ACPI_DEBUG_MSG("DSDT contains " << dec << dsdt->GetAMLCodeLength() << " Byte AML Code");
        }
    
        AmlSDT* ssdt = ACPITable::FromAddress<AmlSDT>(rsdt->GetTable("SSDT"));
        if(ssdt != NULL)
        {
            HAL_ACPI_DEBUG_MSG("Found SSDT");
            ASSERT(ssdt->IsValid(), "SSDT is invalid");
            HAL_ACPI_DEBUG_MSG("SSDT contains " << dec << ssdt->GetAMLCodeLength() << " Byte AML Code");
        }
    
        HPET* hpet = ACPITable::FromAddress<HPET>(rsdt->GetTable("HPET"));
        if(hpet != NULL)
        {
            HAL_ACPI_DEBUG_MSG("Found HPET");
            ASSERT(hpet->IsValid(), "HPET is invalid");
            
            HAL_ACPI_DEBUG_MSG("HPET has number " << hex << hpet->GetHPETNumber() << " and base address " << hpet->GetBaseAddress());
        }
        
        MADT* madt = ACPITable::FromAddress<MADT>(rsdt->GetTable("APIC"));
        if(madt != NULL)
        {
            HAL_ACPI_DEBUG_MSG("Found MADT");
            ASSERT(madt->IsValid(), "MADT is invalid");
            HAL_ACPI_DEBUG_MSG("MADT contains " << dec << madt->GetAPICStructureCount() << " APIC structs");
            
            for(unsigned int i = 0; i < madt->GetAPICStructureCount(); i++)
            {
                uint8_t type;
                APICStructureHeader* apicStruct = madt->GetAPICStruct(i, &type);
                
                if(type == LOCALAPIC)
                {
                    HAL_ACPI_DEBUG_MSG("APIC Struct " << dec << i << " is of type LOCALAPIC");
                    LocalAPICStructure* lapic = (LocalAPICStructure*)apicStruct;
                    HAL_ACPI_DEBUG_MSG("LAPIC Processor ID is " << hex << lapic->ProcessorID);
                }
                else if(type == IOAPIC)
                {
                    HAL_ACPI_DEBUG_MSG("APIC Struct " << dec << i << " is of type IOAPIC");
                    IOAPICStructure* ioapic = (IOAPICStructure*)apicStruct;
                    HAL_ACPI_DEBUG_MSG("IOAPIC ID is " << hex << ioapic->IOAPICID);
                    HAL_ACPI_DEBUG_MSG("IOAPIC Base address is " << hex << ioapic->IOAPICAddress);
                }
            }
        }
    }
}
