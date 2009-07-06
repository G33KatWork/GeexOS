#ifndef PLACEMENT_ALLOCATOR_H_
#define PLACEMENT_ALLOCATOR_H_

#include <lib/types.h>
#include <kernel/Memory/IMemoryAllocator.h>

namespace Memory
{
    class PlacementAllocator : public IMemoryAllocator
    {
    public:
        PlacementAllocator();
        
        void* Allocate(size_t len, bool pageAlign);
        void Deallocate(void* p);

    private:
        unsigned int placement_address;
    };
}
#endif
