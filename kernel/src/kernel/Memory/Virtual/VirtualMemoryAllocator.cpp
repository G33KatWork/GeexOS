#include <kernel/Memory/Virtual/VirtualMemoryAllocator.h>
#include <kernel/global.h>

using namespace Memory;

bool VirtualMemoryAllocator::SwapOutRegion(Address address, size_t size)
{
    if(SwappingAllowed)
        //TODO: Insert swapping here
        return false;
    
    return false;
}

bool VirtualMemoryAllocator::SwapInRegion(Address address, size_t size)
{
    if(!SwappingAllowed)
        PANIC("The fuck? How are we able to swap in a memory region if we are not able to swap it out beforehand?");
        
    return false;
}
