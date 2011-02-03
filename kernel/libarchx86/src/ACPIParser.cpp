#include <arch/internal/ACPIParser.h>
#include <types.h>
#include <arch/internal/ACPI/RSDP.h>

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
    
        #ifdef EN_HAL_ACPI_DEBUG_MSG
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
        #endif
    
        fadt = ACPITable::FromAddress<FADT>(rsdt->GetTable("FACP"));
        ssdt = ACPITable::FromAddress<AmlSDT>(rsdt->GetTable("SSDT"));
        hpet = ACPITable::FromAddress<HPET>(rsdt->GetTable("HPET"));
        madt = ACPITable::FromAddress<MADT>(rsdt->GetTable("APIC"));
        
        if(fadt != NULL && fadt->IsValid())
            dsdt = ACPITable::FromAddress<AmlSDT>(fadt->GetDSDTAddress());
        
        #ifdef EN_HAL_ACPI_DEBUG_MSG
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
        }
        
        if(dsdt != NULL)
        {
            HAL_ACPI_DEBUG_MSG("Found DSDT");
            ASSERT(dsdt->IsValid(), "DSDT is invalid");
            HAL_ACPI_DEBUG_MSG("DSDT contains " << dec << dsdt->GetAMLCodeLength() << " Byte AML Code");
        }
        
        if(ssdt != NULL)
        {
            HAL_ACPI_DEBUG_MSG("Found SSDT");
            ASSERT(ssdt->IsValid(), "SSDT is invalid");
            HAL_ACPI_DEBUG_MSG("SSDT contains " << dec << ssdt->GetAMLCodeLength() << " Byte AML Code");
        }
        
        if(hpet != NULL)
        {
            HAL_ACPI_DEBUG_MSG("Found HPET");
            ASSERT(hpet->IsValid(), "HPET is invalid");
            
            HAL_ACPI_DEBUG_MSG("HPET has number " << hex << hpet->GetHPETNumber() << " and base address " << hpet->GetBaseAddress());
        }
        
        if(madt != NULL)
        {
            HAL_ACPI_DEBUG_MSG("Found MADT");
            ASSERT(madt->IsValid(), "MADT is invalid");
            HAL_ACPI_DEBUG_MSG("MADT contains " << dec << madt->GetAPICStructureCount() << " APIC structs");
            
            for(unsigned int i = 0; i < madt->GetAPICStructureCount(); i++)
            {
                ApicStructureTypes type;
                APICStructureHeader* apicStruct = madt->GetAPICStruct(i, &type);
                
                if(type == MADT_LOCALAPIC)
                {
                    HAL_ACPI_DEBUG_MSG("APIC Struct " << dec << i << " is of type LOCALAPIC");
                    LocalAPICStructure* lapic = (LocalAPICStructure*)apicStruct;
                    HAL_ACPI_DEBUG_MSG("LAPIC Processor ID is " << hex << lapic->ProcessorID);
                }
                else if(type == MADT_IOAPIC)
                {
                    HAL_ACPI_DEBUG_MSG("APIC Struct " << dec << i << " is of type IOAPIC");
                    IOAPICStructure* ioapic = (IOAPICStructure*)apicStruct;
                    HAL_ACPI_DEBUG_MSG("IOAPIC ID is " << hex << ioapic->IOAPICID);
                    HAL_ACPI_DEBUG_MSG("IOAPIC Base address is " << hex << ioapic->IOAPICAddress);
                }
                else if(type == MADT_INTERRUPTSOURCEOVERRIDE)
                {
                    HAL_ACPI_DEBUG_MSG("APIC Struct " << dec << i << " is of type INTERRUPTSOURCEOVERRIDE");
                    InterruptSourceOverrideStructure* intSrcOverride = (InterruptSourceOverrideStructure*)apicStruct;
                    
                    HAL_ACPI_DEBUG_MSG("Overriding interrupt from " << hex << intSrcOverride->Source << " to " << intSrcOverride->GlobalSystemInterrupt << " on Bus " << intSrcOverride->Bus);
                }
                else if(type == MADT_NMISOURCE)
                {
                    HAL_ACPI_DEBUG_MSG("APIC Struct " << dec << i << " is of type NMISOURCE");
                    NMIInterruptSourceStructure* nmiIntSrc = (NMIInterruptSourceStructure*)apicStruct;
                }
                else if(type == MADT_LOCALAPICNMI)
                {
                    HAL_ACPI_DEBUG_MSG("APIC Struct " << dec << i << " is of type LOCALAPICNMI");
                    LocalAPICNMIStructure* localNmi = (LocalAPICNMIStructure*)apicStruct;
                }
                else if(type == MADT_LOCALAPICADDRESSOVERRIDE)
                {
                    HAL_ACPI_DEBUG_MSG("APIC Struct " << dec << i << " is of type LOCALAPICADDRESSOVERRIDE");
                    LocalAPICAddressOverrideStructure* lapic = (LocalAPICAddressOverrideStructure*)apicStruct;
                }
                else if(type == MADT_IOSAPIC)
                {
                    HAL_ACPI_DEBUG_MSG("APIC Struct " << dec << i << " is of type IOSAPIC");
                    IOSAPICStructure* iosapic = (IOSAPICStructure*)apicStruct;
                }
                else if(type == MADT_LOCALSAPIC)
                {
                    HAL_ACPI_DEBUG_MSG("APIC Struct " << dec << i << " is of type LOCALSAPIC");
                    LocalSAPICStructure* lsapic = (LocalSAPICStructure*)apicStruct;
                }
                else if(type == MADT_PLATFORMINTERRUPTSOURCES)
                {
                    HAL_ACPI_DEBUG_MSG("APIC Struct " << dec << i << " is of type PLATFORMINTERRUPTSOURCES");
                    PlatformInterruptSourceStructure* platIntSrc = (PlatformInterruptSourceStructure*)apicStruct;
                }
                else if(type == MADT_LOCALX2APIC)
                {
                    HAL_ACPI_DEBUG_MSG("APIC Struct " << dec << i << " is of type LOCALX2APIC");
                    ProcessorLocalX2APICStructure* x2xapic = (ProcessorLocalX2APICStructure*)apicStruct;
                }
                else if(type == MADT_LOCALX2APICNMI)
                {
                    HAL_ACPI_DEBUG_MSG("APIC Struct " << dec << i << " is of type LOCALX2APICNMI");
                    LocalX2APICNMIStructure* lx2apicNmi = (LocalX2APICNMIStructure*)apicStruct;
                }
            }
        }
        #endif
    }
}
