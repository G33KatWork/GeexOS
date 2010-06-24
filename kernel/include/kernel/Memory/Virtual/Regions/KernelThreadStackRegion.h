#ifndef KERNELTHREADSTACKMEMORYREGION_H_
#define KERNELTHREADSTACKMEMORYREGION_H_

#include <lib/types.h>
#include <kernel/global.h>
#include <kernel/Memory/Virtual/Regions/LazyMemoryRegion.h>

namespace Memory
{
    class KernelThreadStack;
    
    /*  This region maintains all the stacks of kernel threads
     */
    class KernelThreadStackMemoryRegion : public LazyMemoryRegion
    {
        friend class VirtualMemorySpace;
    private:
        size_t initialSize;
        size_t curSize;
    
    public:
        KernelThreadStackMemoryRegion(Address RegionStart, size_t MaxRegionSize, const char* RegionName)´
            : LazyMemoryRegion(RegionStart, MaxRegionSize, RegionName, ALLOCFLAG_WRITABLE)
        {}
        
        KernelThreadStack* CreateStack(size_t InitialSize, size_t MaxSize)
        {
        }
        
        void DestroyStack(KernelThreadStack* stack)
        {
        }
        
        virtual bool HandlePageFault()
		{
		    //TODO: Determine the corresponding thread stack and expand it appropriately
		    //      alternatively we need to panic here. We can't hand this fault up
		    //      because LazyMemoryRegion doesn't know about our stacks here
		    
		    //We handle this stuff here or we are in kernel panic land, so return true
            return true;
		}
    };
    
    class KernelThreadStack
    {
    private:
        Address beginning;
        size_t curSize;
        size_t maxSize;
    };
}
#endif
