#ifndef PLACEMENT_ALLOCATOR_H_
#define PLACEMENT_ALLOCATOR_H_

#include <lib/types.h>

namespace Memory
{
    class PlacementAllocator
    {
    public:
        PlacementAllocator();
        
        void* Allocate(size_t len, bool pageAlign);

    private:
        unsigned int placement_address;
    };
}
#endif
