#ifndef STATICMEMORYALLOCATOR_H_
#define STATICMEMORYALLOCATOR_H_

#include <lib/types.h>
#include <arch/paging.h>
#include <kernel/Memory/Virtual/VirtualMemoryAllocator.h>
#include <kernel/Memory/Virtual/VirtualMemoryRegion.h>

namespace Memory
{       
    /*
     * This class defines a static memory allocator which should be used
     * for memory which already is allocated but must now be tracked by the
     * virtual memory management subsystem.
     * Essentially we are not mapping anything here, but updating the flags
     * in the current active paging.
     *
     * Normally these regions are only kernel regions which are created during
     * kernel startup, like .text, .data, .bss, .rodata and .placement
     */
    class StaticMemoryAllocator : public VirtualMemoryAllocator
    {
    public:
        StaticMemoryAllocator();
        
        virtual void* Allocate(VirtualMemoryRegion* region, Address address, size_t size, AllocationFlags flags);
        virtual void Deallocate(VirtualMemoryRegion* region, Address address);
        
        /*
         * Since this memory is already allocated and probably being used at
         * the time when creating this Allocator, we must have a way to set
         * several flags in the memory subsystem, if it didn't happen before.
         * These flags are mainly Paging-Related: User-/Kernelmode, Executable, RW etc.
         */
        void CorrectAllocationFlags(VirtualMemoryRegion* region, AllocationFlags flags);
     
    private:
    };
}
#endif
