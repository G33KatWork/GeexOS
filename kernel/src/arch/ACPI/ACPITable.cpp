#include <arch/ACPI/ACPITable.h>
#include <kernel/debug.h>
#include <kernel/global.h>
#include <lib/string.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <kernel/Memory/IO/IOMemoryManager.h>

using namespace Arch::ACPI;
using namespace IO;
using namespace Memory;

bool ACPITable::ChecksumValid(Address start, size_t len)
{
    char checksum = 0;
    char* ptr = (char*)start;
    for(unsigned int i = 0; i < len; i++)
        checksum += ptr[i];
    
    return checksum == 0;
}

Address ACPITable::TranslatePhysical(Address physical)
{
    return VirtualMemoryManager::GetInstance()->IOMemory()->TranslatePhysicalAddress(physical);
}
