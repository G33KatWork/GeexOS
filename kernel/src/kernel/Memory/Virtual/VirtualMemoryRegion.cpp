#include <kernel/Memory/Virtual/VirtualMemoryRegion.h>

using namespace Memory;

VirtualMemoryRegion::VirtualMemoryRegion(Address RegionStart, size_t RegionSize, const char* RegionName)
{
    if(!IS_PAGE_ALIGNED(RegionStart))
        PANIC("Start address needs to be page aligned.");
    
    StartAddress = RegionStart;
    Size = RegionSize;
    Name = RegionName;
    Next = NULL;
    Allocator = NULL;
}
