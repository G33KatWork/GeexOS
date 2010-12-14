#include <arch/internal/ACPI/RSDT.h>
#include <string.h>

using namespace Arch::ACPI;

Address RSDT::GetTable(const char* SubtableSignature)
{
    for(unsigned int i = 0; i < GetSubtableCount(); i++)
    {
        ACPITable h = ACPITable(TranslatePhysical((Address)Read32(ACPITABLEHEADERLEN + i * 4)));
        if(memcmp(h.Signature(), SubtableSignature, 4) == 0)
            return h.GetAddress();
    }

    return NULL;
}

Address RSDT::GetTable(unsigned int index)
{
    if(index > GetSubtableCount()) return NULL;
    Address a = (Address)Read32(ACPITABLEHEADERLEN + index * 4);
    return TranslatePhysical(a);
}

Address XSDT::GetTable(const char* SubtableSignature)
{
    for(unsigned int i = 0; i < GetSubtableCount(); i++)
    {
        ACPITable h = ACPITable(TranslatePhysical((Address)Read32(ACPITABLEHEADERLEN + i * 8)));
        if(memcmp(h.Signature(), SubtableSignature, 4) == 0)
            return h.GetAddress();
    }

    return NULL;
}

Address XSDT::GetTable(unsigned int index)
{   
    if(index > GetSubtableCount()) return NULL;
    return TranslatePhysical((Address)Read64(ACPITABLEHEADERLEN + index * 8));
}
