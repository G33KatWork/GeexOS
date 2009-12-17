#ifndef VIRTUALMEMORYREGION_H_
#define VIRTUALMEMORYREGION_H_

#include <lib/types.h>
#include <kernel/Memory/VirtualMemoryAllocator.h>

namespace Memory
{
    /*
     * This class defines a region inside a virtual memory space
     * All memory in this region will be allocated either by a
     * manual call of the Allocator or by the PageFaultHandler
     */
    class VirtualMemoryRegion
    {
    public:
        VirtualMemoryRegion(Address RegionStart, size_t RegionSize, char* RegionName, VirtualMemoryRegion* NextRegion)
        {
            StartAddress = RegionStart;
            Size = RegionSize;
            Name = RegionName;
            Next = NextRegion;
        }
        
        Address StartAddress;
        size_t Size;
        char* Name;
        VirtualMemoryRegion *Next;
        VirtualMemoryAllocator *Allocator;
    };
}
#endif
