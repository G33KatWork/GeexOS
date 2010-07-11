#ifndef PREALLOCATEDMEMORYREGION_H_
#define PREALLOCATEDMEMORYREGION_H_

#include <types.h>
#include <arch/types.h>
#include <kernel/global.h>
#include <kernel/Memory/Virtual/VirtualMemoryRegion.h>

namespace Memory
{
    class PreallocatedMemoryRegion : public VirtualMemoryRegion
    {
        friend class VirtualMemorySpace;
        
    public:
    
    protected:
        PreallocatedMemoryRegion(Address RegionStart, size_t RegionSize, const char* RegionName, AllocationFlags RegionFlags)
        : VirtualMemoryRegion(RegionStart, RegionSize, RegionName, RegionFlags)
        {}
    };
}
#endif
