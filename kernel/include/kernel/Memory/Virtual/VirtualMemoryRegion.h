#ifndef VIRTUALMEMORYREGION_H_
#define VIRTUALMEMORYREGION_H_

#include <lib/types.h>
#include <kernel/Memory/Virtual/VirtualMemoryAllocator.h>

namespace Memory
{
    /*
     * This class defines a region inside a virtual memory space
     * All memory in this region will be allocated either by a
     * manual call of the Allocator or by the PageFaultHandler
     */
    class VirtualMemoryRegion
    {
    friend class VirtualMemorySpace;
        
    public:
        VirtualMemoryRegion(Address RegionStart, size_t RegionSize, const char* RegionName);
        
        Address StartAddress;
        size_t Size;
        const char* Name;
        VirtualMemoryAllocator *Allocator;
    
    private:
        VirtualMemoryRegion *Next;
    };
}
#endif
