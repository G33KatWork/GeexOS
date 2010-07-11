#ifndef LAZYMEMORYREGION_H_
#define LAZYMEMORYREGION_H_

#include <types.h>
#include <arch/types.h>
#include <kernel/global.h>
#include <kernel/Memory/Virtual/VirtualMemoryRegion.h>

namespace Memory
{
    class LazyMemoryRegion : public VirtualMemoryRegion
    {
        friend class VirtualMemorySpace;
        
    public:
        LazyMemoryRegion(Address RegionStart, size_t RegionSize, const char* RegionName, AllocationFlags RegionFlags)
        : VirtualMemoryRegion(RegionStart, RegionSize, RegionName, RegionFlags)
        {}
        
        virtual void DoSwapping(){}
        
		virtual bool HandlePageFault()
		{
		    //TODO: Expand region if possible - demand paging and stuff...
		    
		    if(false) //not able to handle here
		        VirtualMemoryRegion::HandlePageFault(); //hand up
		    
            return true;
		}
    };
}
#endif
