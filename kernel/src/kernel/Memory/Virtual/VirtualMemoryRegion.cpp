#include <kernel/Memory/Virtual/VirtualMemoryRegion.h>
#include <arch/paging.h>

using namespace Memory;
using namespace IO;

VirtualMemoryRegion::VirtualMemoryRegion(Address RegionStart, size_t RegionSize, const char* RegionName)
{
    ASSERT(IS_PAGE_ALIGNED(RegionStart), "Start address needs to be page aligned.");
    
    startAddress = RegionStart;
    size = RegionSize;
    name = RegionName;
    Next = NULL;
    SwappingAllowed = false;
}
