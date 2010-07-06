#include <arch/APIC.h>
#include <kernel/global.h>
#include <kernel/debug.h>
#include <arch/cpuid.h>
#include <arch/msr.h>

using namespace IO;
using namespace Arch;

bool Arch::CPUsupportsAPIC(void)
{
    uint32_t a, d;
    cpuid(CPUID_GETFEATURES, &a, &d);
    return d & CPUID_FLAG_APIC;
}

uint32_t APIC::readIOApic(Address addr, uint32_t reg)
{
    volatile uint32_t* ioapic = (volatile uint32_t*)addr;
    ioapic[0] = reg;
    return ioapic[4];
}

void APIC::writeIOApic(Address addr, uint32_t reg, uint32_t val)
{
    volatile uint32_t* ioapic = (volatile uint32_t*)addr;
    ioapic[0] = reg;
    ioapic[4] = val;
}

void APIC::SetBaseAddress(Address newBase)
{
    Address a = newBase & 0xFFFFF000 | 0x800;
    setMSR(MSR_APIC_BASE, a, 0);
}

Address APIC::GetBaseAddress(void)
{
    Address a, d;
    getMSR(MSR_APIC_BASE, &a, &d);
    return a & 0xFFFFF000;
}
