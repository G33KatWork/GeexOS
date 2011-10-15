#include <kernel/Memory/PlacementAllocator.h>
#include <arch/HAL.h>

using namespace Memory;

extern unsigned int placement;

PlacementAllocator::PlacementAllocator()
{
    //See the linker script for an exact location of placement stuff
    placement_address = (unsigned int)&placement;
}

void* PlacementAllocator::Allocate(size_t len, bool pageAlign)
{
    //PLACEMENT_DEBUG_MSG("Allocating " << dec << len << " Bytes " << (pageAlign?" page-aligned":""));
    //PLACEMENT_DEBUG_MSG("Currently allocated space in placement region: " << dec << GetPlacementAllocatedSize() << " Bytes");
    
    if(GetPlacementAllocatedSize() + len > KERNEL_PLACEMENT_SIZE)
        PANIC("Placement region overflow");
    
    unsigned int tmp;

    if(pageAlign && (placement_address % PAGE_SIZE != 0))
    {
        // Align the placement address;
        placement_address &= PAGEALIGN_MASK;
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

size_t PlacementAllocator::GetPlacementAllocatedSize()
{
    return placement_address - (unsigned int)&placement;
}

Address Memory::GetPlacementBeginning()
{
    return (Address)&placement;
}
