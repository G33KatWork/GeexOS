#ifndef VIRTUALMEMORYALLOCATOR_H_
#define VIRTUALMEMORYALLOCATOR_H_

#include <lib/types.h>

namespace Memory
{
    //Avoid circular dependency with VirtualMemoryRegion
    class VirtualMemoryRegion;
    
    struct _AllocationFlags
    {
        unsigned int usermode:1;
        unsigned int writable:1;
        unsigned int executable:1;
        
        unsigned int unused:29;
    };
    typedef struct _AllocationFlags AllocationFlags;
        
    /*
     * This class defines the source of space of a MemoryObject
     * This can be anonymous memory, a file or whatever
     * Swapping can be handled here, too.
     */
    class VirtualMemoryAllocator
    {
    public:
        VirtualMemoryAllocator() { SwappingAllowed = true; }
        virtual ~VirtualMemoryAllocator() {}
        
        virtual void* Allocate(VirtualMemoryRegion* region, Address address, size_t size, AllocationFlags flags) = 0;
        virtual void Deallocate(VirtualMemoryRegion* region, Address address) = 0;
        
        bool SwapOutRegion(Address address, size_t size);
        bool SwapInRegion(Address address, size_t size);
    
    protected:
        //Set this variable to false is no swapping should ever occour
        //Standard is true
        bool SwappingAllowed;
    };
}
#endif
