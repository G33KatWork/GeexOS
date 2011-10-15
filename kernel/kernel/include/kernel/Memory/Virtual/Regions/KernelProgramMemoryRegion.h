#ifndef KERNELPROGRAMMEMORYREGION_H_
#define KERNELPROGRAMMEMORYREGION_H_

#include <types.h>
#include <kernel/Memory/Virtual/VirtualMemoryRegion.h>

namespace Memory
{
    /**
     * This memory region is used to tell the memory manager about the memory regions
     * the loader has already mapped for us when loading the kernel.
     * All flags inside the paging are already set properly, so the class only needs to
     * mark the occupied memory used inside the physical memory manager in the HAL.
    **/
    class KernelProgramMemoryRegion : public VirtualMemoryRegion
    {
    public:
        KernelProgramMemoryRegion(Address RegionStart, size_t RegionSize, const char* RegionName, AllocationFlags RegionFlags);
    
    private:
        ~KernelProgramMemoryRegion() {}
    };
}
#endif
