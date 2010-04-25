#include <kernel/Memory/PlacementAllocator.h>
#include <kernel/global.h>
#include <arch/Paging.h>
#include <kernel/debug.h>

using namespace Memory;
using namespace IO;

extern unsigned int placement;

PlacementAllocator::PlacementAllocator()
{
    //See the linker script for an exact location of placement stuff
    placement_address = (unsigned int)&placement;
}

void* PlacementAllocator::Allocate(size_t len, bool pageAlign)
{
    PLACEMENT_DEBUG_MSG("Allocating " << dec << len << " Bytes " << (pageAlign?" page-aligned":""));
    unsigned int tmp;

    if(pageAlign && (placement_address % PAGE_SIZE != 0))
    {
        // Align the placement address;
        placement_address &= IDENTITY_POSITION;
        placement_address += PAGE_SIZE;
    }

    tmp = placement_address;
    placement_address += len;
        
    return (void*)tmp;
}

unsigned int PlacementAllocator::GetPointerPosition()
{
    return placement_address;
} 

Address Memory::GetPlacementBeginning()
{
    return (Address)&placement;
}
