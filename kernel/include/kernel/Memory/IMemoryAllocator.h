#include <lib/types.h>

#ifndef IMEMORY_ALLOCATOR_H
#define IMEMORY_ALLOCATOR_H

namespace Memory
{
    //Interface for MemoryAllocators
    class IMemoryAllocator
    {
    protected:
        virtual ~IMemoryAllocator() {};

    public:    
        virtual void* Allocate(size_t len, bool pageAlign) = 0;
        virtual void Deallocate(void* p) = 0;
    };
}

#endif
