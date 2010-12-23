#include <arch/internal/IOAPIC.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <arch/HAL.h>

using namespace Arch;
using namespace Memory;

void IOAPIC::MapIntoIOMemory()
{
    VirtualMemoryManager::GetInstance()->PhysicalAllocator()->MarkAsUsed(baseAddress);
    VirtualMemoryManager::GetInstance()->IOMemory()->MapPhysical(baseAddress, 0x1000, "Local APIC");
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
