#include <arch/internal/IOAPIC.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <arch/x86InterruptDispatcher.h>
#include <arch/HAL.h>

using namespace Arch;
using namespace Arch::ACPI;
using namespace Memory;

void IOAPIC::MapIntoIOMemory()
{
    VirtualMemoryManager::GetInstance()->PhysicalAllocator()->MarkAsUsed(baseAddress);
    VirtualMemoryManager::GetInstance()->IOMemory()->MapPhysical(baseAddress, 0x1000, "Local APIC");
}

void IOAPIC::Initialize()
{
    //Setup standard mapping
    for(unsigned int i = 0; i < 256; i++)
        irqToAPICPin[i] = i;
    
    maxRedirectionEntry = (IOAPICRead32(IOAPIC_VERSION) >> IOAPIC_MAX_REDIRECTION_ENTRY_SHIFT) & IOAPIC_MAX_REDIRECTION_ENTRY_MASK;
    DEBUG_MSG("IOAPIC has " << Debug::dec << maxRedirectionEntry+1 << " redirection entries");
    
    //TODO: Deliver interrupts to other processors
    //uint64_t targetLAPIC = ((x86Processor*)CurrentHAL->GetCurrentProcessor())->GetLAPIC()->GetID();
    
    for(int i = 0; i < maxRedirectionEntry; i++)
        MaskVector(i);
    
    //TODO: handle PCI interrupts
    //FIXME: Singularity masks ALL available interrupt vectors, maybe we should do this too?
    // for(uint32_t i = 0; i < 16/*maxRedirectionEntry*/; i++)
    // {
    //     uint64_t entry = (targetLAPIC << IOAPIC_DESTINATION_FIELD_SHIFT)
    //                    | (IOAPIC_INTERRUPT_MASKED << IOAPIC_INTERRUPT_MASK_SHIFT)
    //                    | ((i + ARCH_X86_INTERRUPT_BASE) << IOAPIC_INTERRUPT_VECTOR_SHIFT);
    //     
    //     //FIXME: why?
    //     /*if(i == 0)
    //     {
    //         // redirection entry 0 is external interrupt
    //         entry |= (IOAPIC_TRIGGER_MODE_EDGE << IOAPIC_TRIGGER_MODE_SHIFT)
    //                 | (IOAPIC_PIN_POLARITY_HIGH_ACTIVE << IOAPIC_PIN_POLARITY_SHIFT)
    //                 | (IOAPIC_DELIVERY_MODE_EXT_INT << IOAPIC_DELIVERY_MODE_SHIFT);
    //     }
    //     else */if(i < 16)
    //     {
    //         // interrupt 1 to 15 are legacy isa interrupts
    //         entry |= (IOAPIC_TRIGGER_MODE_EDGE << IOAPIC_TRIGGER_MODE_SHIFT)
    //                 | (IOAPIC_PIN_POLARITY_HIGH_ACTIVE << IOAPIC_PIN_POLARITY_SHIFT)
    //                 | (IOAPIC_DELIVERY_MODE_FIXED << IOAPIC_DELIVERY_MODE_SHIFT);
    //     }
    //     /*else
    //     {
    //         TODO: handle PCI interrupts 
    //     }*/
    //     
    //     IOAPICWrite64(IOPAIC_REDIRECTION_TABLE_BEGIN + i * 2, entry);
    // }
}

void IOAPIC::HandleInterruptOverride(uint8_t standardVector, uint8_t overriddenVector, InterruptSourceOverrideStructureFlags flags)
{
    //irqToAPICPin[standardVector] = overriddenVector;
    
    //TODO: reconfigure IOAPIC
    //TODO: handle flags
}

void IOAPIC::MaskVector(uint8_t vectorNumber)
{
    uint32_t pin = irqToAPICPin[vectorNumber];
    if(pin > maxRedirectionEntry)
        return;
    
    uint64_t entry = IOAPICRead64(IOPAIC_REDIRECTION_TABLE_BEGIN + pin * 2);
    entry &= ~(1 << IOAPIC_INTERRUPT_MASK_SHIFT);
    entry |= IOAPIC_INTERRUPT_MASKED << IOAPIC_INTERRUPT_MASK_SHIFT;
    IOAPICWrite64(IOPAIC_REDIRECTION_TABLE_BEGIN + pin * 2, entry);
}

void IOAPIC::UnmaskVector(uint8_t vectorNumber)
{
    uint32_t pin = irqToAPICPin[vectorNumber];
    if(pin > (int32_t)maxRedirectionEntry)
        return;
    
    uint64_t entry = IOAPICRead64(IOPAIC_REDIRECTION_TABLE_BEGIN + pin * 2);
    entry &= ~(1 << IOAPIC_INTERRUPT_MASK_SHIFT);
    entry |= IOAPIC_INTERRUPT_UNMASKED << IOAPIC_INTERRUPT_MASK_SHIFT;
    IOAPICWrite64(IOPAIC_REDIRECTION_TABLE_BEGIN + pin * 2, entry);
}

Address IOAPIC::GetVirtBaseAddress()
{
    Address virtMapped = VirtualMemoryManager::GetInstance()->IOMemory()->TranslatePhysicalAddress(baseAddress);
    if(!virtMapped)
        PANIC("Physical IOAPIC address couldn't be translated to IO Memory Space");

    return virtMapped;
}

bool IOAPIC::EndOfInterrupt(uint8_t vectorNumber)
{
    //just always let the dispatcher ACK the interrupt on the local APIC
    return false;
}
