#ifndef PREALLOCATEDMEMORYREGION_H_
#define PREALLOCATEDMEMORYREGION_H_

#include <types.h>
#include <kernel/global.h>
#include <kernel/Memory/Virtual/VirtualMemoryRegion.h>

namespace Memory
{
    class PreallocatedMemoryRegion : public VirtualMemoryRegion
    {
        friend class VirtualMemorySpace;
        
    public:
        virtual void* AllocateMemory(size_t UNUSED(size)) { PANIC("You are not supposed to allocate memory from this region."
                                                                  "Everything in here already is allocated or at least it should be.");
                                                            return NULL;
                                                          }
        virtual void DeallocateMemory(void* UNUSED(beginning)){};
    
    protected:
        PreallocatedMemoryRegion(Address RegionStart, size_t RegionSize, const char* RegionName, AllocationFlags RegionFlags)
        : VirtualMemoryRegion(RegionStart, RegionSize, RegionName, RegionFlags)
        {}
    };
}
#endif
