#include <kernel/Memory/Virtual/VirtualMemoryRegion.h>
#include <arch/Paging.h>

using namespace Memory;
using namespace Arch;
using namespace IO;

VirtualMemoryRegion::VirtualMemoryRegion(Address RegionStart, size_t RegionSize, const char* RegionName)
{
    ASSERT(IS_PAGE_ALIGNED(RegionStart), "Start address needs to be page aligned.");
    
    StartAddress = RegionStart;
    Size = RegionSize;
    Name = RegionName;
    Next = NULL;
    SwappingAllowed = false;
}

void VirtualMemoryRegion::SetFlags(AllocationFlags f)
{
    VIRTUAL_MEMORY_REGION_DEBUG_MSG("Correcting alloc flags for " << Name);
    
    flags = f;
    
    VIRTUAL_MEMORY_REGION_DEBUG_MSG("Starting address: " << hex << (unsigned)StartAddress);
    VIRTUAL_MEMORY_REGION_DEBUG_MSG("Size: " << hex << (unsigned)Size);
    VIRTUAL_MEMORY_REGION_DEBUG_MSG("New flags: " << (f & ALLOCFLAG_WRITABLE?"rw":"ro") <<
                                    " " << (f & ALLOCFLAG_USERMODE?"umode":"kmode") << 
                                    " " << (f & ALLOCFLAG_EXECUTABLE?"exec":"noexec"));
    
    for(Address i = StartAddress; i < PAGE_ALIGN(StartAddress + Size); i += PAGE_SIZE)
    {
        Paging::GetInstance()->MapAddress(
            i,
            Paging::GetInstance()->GetPhysicalAddress(i),
            f & ALLOCFLAG_WRITABLE,
            f & ALLOCFLAG_USERMODE
        );
    }
}

bool VirtualMemoryRegion::SwapOutRegion(Address address, size_t size)
{
    return false;
}

bool VirtualMemoryRegion::SwapInRegion(Address address, size_t size)
{
    return false;
}
