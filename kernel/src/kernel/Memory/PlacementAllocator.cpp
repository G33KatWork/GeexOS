#include <kernel/Memory/PlacementAllocator.h>
#include <kernel/global.h>

using namespace Memory;

extern unsigned int end;

PlacementAllocator::PlacementAllocator()
{
    //FIXME: Why +0x20000? if not, it would overwrite the ELF tables used by stacktraces
    placement_address = (unsigned int)&end + 0x20000;
}

void* PlacementAllocator::Allocate(size_t len, bool pageAlign)
{
    unsigned int tmp;

    if(pageAlign && (placement_address & 0xFFFFF000))
    {
        // Align the placement address;
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }

    tmp = placement_address;
    placement_address += len;
        
    return (void*)tmp;
}

void PlacementAllocator::Deallocate(void* UNUSED(p))
{
    
}
