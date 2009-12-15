#include <kernel/Memory/PlacementAllocator.h>
#include <kernel/global.h>
#include <arch/Paging.h>

using namespace Memory;

extern unsigned int end;

PlacementAllocator::PlacementAllocator()
{
    //FIXME: Why +0x20000? if not, it would overwrite the ELF tables used by stacktraces
    //FIXME: Just move this thing to somewhere else...
    placement_address = (unsigned int)&end + 0x20000;
}

void* PlacementAllocator::Allocate(size_t len, bool pageAlign)
{
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
