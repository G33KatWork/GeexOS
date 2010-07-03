#include <kernel/Memory/Virtual/VirtualMemoryRegion.h>
#include <arch/Paging.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <kernel/debug.h>

using namespace Memory;
using namespace Arch;
using namespace IO;

void VirtualMemoryRegion::MapFreshPages(Address start, size_t length)
{
    VIRTUAL_MEMORY_REGION_DEBUG_MSG("Mapping pages into virtual memory region " << name << endl
                                    << "Start: " << start << endl
                                    << "Size: " << (unsigned)length << endl
                                    );
    
    ASSERT(IS_PAGE_ALIGNED(start) && IS_PAGE_ALIGNED(length), "Start address and length of region to be equipped with fresh pages must be page aligned");
    
    if(start < startAddress || start + length > startAddress + size)
        PANIC("Arguments out of bounds");
    
    for(Address i = start; i < start + length; i += PAGE_SIZE)
        Paging::GetInstance()->MapAddress(
            i,
            VirtualMemoryManager::GetInstance()->PhysicalAllocator()->AllocateFrame(),
            flags & ALLOCFLAG_WRITABLE,
            flags & ALLOCFLAG_USERMODE 
        );
}

void VirtualMemoryRegion::UnmapPages(Address start, size_t length)
{
    VIRTUAL_MEMORY_REGION_DEBUG_MSG("Unmapping pages from virtual memory region " << name << endl
                                    << "Start: " << start << endl
                                    << "Size: " << (unsigned)length << endl
                                    );
    
    ASSERT(IS_PAGE_ALIGNED(start) && IS_PAGE_ALIGNED(length), "Start address and length of region to be equipped with fresh pages must be page aligned");

    if(start < startAddress || start + length > startAddress + size)
        PANIC("Arguments out of bounds");
    
    for(Address i = start; i < start + length; i += PAGE_SIZE)
    {
        Address phys = Paging::GetInstance()->GetPhysicalAddress(i);
        Paging::GetInstance()->UnmapAddress(i);
        VirtualMemoryManager::GetInstance()->PhysicalAllocator()->DeallocateFrame(phys);
    }
}
