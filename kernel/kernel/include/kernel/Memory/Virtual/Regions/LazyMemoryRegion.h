#ifndef LAZYMEMORYREGION_H_
#define LAZYMEMORYREGION_H_

#include <types.h>
#include <kernel/global.h>
#include <kernel/Memory/Virtual/VirtualMemoryRegion.h>
#include <arch/HAL.h>

namespace Memory
{
    class LazyMemoryRegion : public VirtualMemoryRegion
    {
        friend class VirtualMemorySpace;
        
    public:
        LazyMemoryRegion(Address RegionStart, size_t RegionSize, const char* RegionName, AllocationFlags RegionFlags)
        : VirtualMemoryRegion(RegionStart, RegionSize, RegionName, RegionFlags)
        {}
        
		virtual bool HandlePageFault(Address faultingAddress)
		{
            VIRTUAL_MEMORY_REGION_DEBUG_MSG("Pagefault handler in LazyMemoryRegion called with faulting address "
                                            << Debug::hex << faultingAddress << ". Expanding region by one page.");
            
            if(Arch::CurrentHAL->GetPaging()->IsPresent(faultingAddress)) //not able to handle here
		    {
                VIRTUAL_MEMORY_REGION_DEBUG_MSG("Faulting address is already present. Can't expand region.");
		        VirtualMemoryRegion::HandlePageFault(faultingAddress); //hand up
		    }
		    else
                MapFreshPages(faultingAddress & PAGEALIGN_MASK, PAGE_SIZE); //else map another page into this region
		    
            return true;
		}
    };
}
#endif
