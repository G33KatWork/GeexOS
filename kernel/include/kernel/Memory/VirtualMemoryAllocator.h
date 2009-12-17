#ifndef VIRTUALMEMORYALLOCATOR_H_
#define VIRTUALMEMORYALLOCATOR_H_

#include <lib/types.h>

namespace Memory
{
    /*
     * This class defines the source of space of a MemoryObject
     * This can be anonymous memory, a file or whatever
     * Swapping can be handled here, too.
     */
    class VirtualMemoryAllocator
    {
    public:
        VirtualMemoryAllocator(unsigned int allocationFlags){}
        
        void* Allocate(Address address, size_t size){ return NULL; }
        void Deallocate(Address address){}
        
        void SwapOutRegion(Address address, size_t size){}
        void SwapInRegion(Address address, size_t size){}
    };
}
#endif
