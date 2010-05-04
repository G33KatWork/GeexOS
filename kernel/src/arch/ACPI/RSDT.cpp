#include <arch/ACPI/RSDT.h>
#include <lib/string.h>
#include <kernel/debug.h>

using namespace Arch::ACPI;

Address RSDT::GetTable(const char* Signature)
{
    for(unsigned int i = 0; i < GetSubtableCount(); i++)
    {
        ACPITableHeader *h = (ACPITableHeader *)TranslatePhysical(((struct RSDTDescriptor*)descriptor)->NextSDT[i]);
        if(h != NULL && memcmp(h->Signature, Signature, 4) == 0)
            return (Address)h;
    }

    return NULL;
}

Address RSDT::GetTable(unsigned int index)
{
    if(index > GetSubtableCount()) return NULL;
    return TranslatePhysical(((struct RSDTDescriptor*)descriptor)->NextSDT[index]);
}

Address XSDT::GetTable(const char* Signature)
{
    for(unsigned int i = 0; i < GetSubtableCount(); i++)
    {
        ACPITableHeader *h = (ACPITableHeader *)TranslatePhysical(((struct XSDTDescriptor*)descriptor)->NextSDT[i]);
        if(h != NULL && memcmp(h->Signature, Signature, 4) == 0)
            return (Address)h;
    }

    return NULL;
}

Address XSDT::GetTable(unsigned int index)
{   
    if(index > GetSubtableCount()) return NULL;
    return TranslatePhysical(((struct XSDTDescriptor*)descriptor)->NextSDT[index]);
}
