#include <arch/internal/ACPI/ACPITable.h>
#include <string.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>

using namespace Arch::ACPI;
using namespace Memory;

char* ACPITable::Read(size_t offset, size_t length)
{
	ASSERT(offset + length <= GetLengthInternal(), "Offset out of Bounds");
	return (char*)(tableStart + offset);
}

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
