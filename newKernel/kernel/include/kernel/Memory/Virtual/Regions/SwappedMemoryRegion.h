#ifndef SWAPPEDMEMORYREGION_H_
#define SWAPPEDMEMORYREGION_H_

#include <types.h>
#include <kernel/global.h>
#include <kernel/Memory/Virtual/Regions/LazyMemoryRegion.h>

namespace Memory
{
    class SwappedMemoryRegion : public LazyMemoryRegion
    {
        friend class VirtualMemorySpace;
        
    public:
        SwappedMemoryRegion(Address RegionStart, size_t RegionSize, const char* RegionName, AllocationFlags RegionFlags)
            : LazyMemoryRegion(RegionStart, RegionSize, RegionName, RegionFlags)
        {}
        
        virtual void DoSwapping()
        {
            //TODO: Swap stuff out...
        }
        
		virtual bool HandlePageFault(Address faultingAddress)
		{
            VIRTUAL_MEMORY_REGION_DEBUG_MSG("Pagefault handler in PagedMemoryRegion called");
		    //TODO: Swap stuff in if possible or panic/kill process etc. 
		    
		    if(false)   //not able to handle here...
		        return LazyMemoryRegion::HandlePageFault(faultingAddress); //hand up
		    
            return true;
		}
    };
}
#endif
