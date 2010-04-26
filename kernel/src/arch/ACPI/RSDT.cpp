#include <arch/ACPI/RSDT.h>
#include <lib/string.h>

using namespace Arch::ACPI;

Address RSDT::GetTable(const char* Signature)
{
    for(unsigned int i = 0; i < GetSubtableCount(); i++)
    {
        ACPITableHeader *h = (ACPITableHeader *)descriptor->NextSDT[i];
        if(memcmp(h->Signature, Signature, 4) == 0)
            return (Address)h;
    }

    return NULL;
}
#include <kernel/debug.h>
Address RSDT::GetTable(unsigned int index)
{   
    if(index > GetSubtableCount()) return NULL;
    return descriptor->NextSDT[index];
}
