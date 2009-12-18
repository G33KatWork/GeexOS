#include <kernel/Memory/VirtualMemoryRegion.h>

using namespace Memory;

VirtualMemoryRegion::VirtualMemoryRegion(Address RegionStart, size_t RegionSize, const char* RegionName)
{
    StartAddress = RegionStart;
    Size = RegionSize;
    Name = RegionName;
    Next = NULL;
}
