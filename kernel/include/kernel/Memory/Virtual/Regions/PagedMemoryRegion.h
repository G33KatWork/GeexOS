#ifndef PAGEDMEMORYREGION_H_
#define PAGEDMEMORYREGION_H_

#include <lib/types.h>
#include <kernel/global.h>
#include <kernel/Memory/Virtual/Regions/LazyMemoryRegion.h>

namespace Memory
{
    class PagedMemoryRegion : public LazyMemoryRegion
    {
        friend class VirtualMemorySpace;
        
    public:
        PagedMemoryRegion(Address RegionStart, size_t RegionSize, const char* RegionName, AllocationFlags RegionFlags)
            : LazyMemoryRegion(RegionStart, RegionSize, RegionName, RegionFlags)
        {}
        
        virtual void* AllocateMemory(size_t UNUSED(size))
        {
            return NULL;
        }
        
        virtual void DeallocateMemory(void* UNUSED(beginning))
        {
            
        }
        
        virtual void DoSwapping()
        {
            //TODO: Swap stuff out...
        }
        
		virtual bool HandlePageFault()
		{
		    //TODO: Swap stuff in if possible or panic/kill process etc. 
		    
		    if(false)   //not able to handle here...
		        return LazyMemoryRegion::HandlePageFault(); //hand up
		    
            return true;
		}
    };
}
#endif
