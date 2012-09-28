#include <kernel/Memory/Virtual/VirtualMemoryRegion.h>
#include <arch/HAL.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <kernel/debug.h>

using namespace Memory;
using namespace Arch;
using namespace Debug;

void VirtualMemoryRegion::MapFreshPages(Address start, size_t length)
{
    VIRTUAL_MEMORY_REGION_DEBUG_MSG("Mapping pages into virtual memory region " << name
                                    << " Start: " << start
                                    << " Size: " << (unsigned)length << endl
                                    );
    
    ASSERT(IS_PAGE_ALIGNED(start) && IS_PAGE_ALIGNED(length), "Start address and length of region to be equipped with fresh pages must be page aligned");
    
    if(start < startAddress || start + length > startAddress + size)
        PANIC("Arguments out of bounds");
    
    for(Address i = start; i < start + length; i += PAGE_SIZE)
        CurrentHAL->GetPaging()->MapAddress(
            i,
            CurrentHAL->GetPhysicalMemoryAllocator()->AllocateFrame(),
            flags & ALLOCFLAG_WRITABLE,
            flags & ALLOCFLAG_USERMODE 
        );
}

void VirtualMemoryRegion::UnmapPages(Address start, size_t length)
{
    VIRTUAL_MEMORY_REGION_DEBUG_MSG("Unmapping pages from virtual memory region " << name
                                    << " Start: " << start
                                    << " Size: " << (unsigned)length << endl
                                    );
    
    ASSERT(IS_PAGE_ALIGNED(start) && IS_PAGE_ALIGNED(length), "Start address and length of region to be equipped with fresh pages must be page aligned");

    if(start < startAddress || start + length > startAddress + size)
        PANIC("Arguments out of bounds");
    
    for(Address i = start; i < start + length; i += PAGE_SIZE)
    {
        Address phys = CurrentHAL->GetPaging()->GetPhysicalAddress(i);
        if(phys)
        {
            CurrentHAL->GetPaging()->UnmapAddress(i);
            CurrentHAL->GetPhysicalMemoryAllocator()->DeallocateFrame(phys);
        }
    }
}
