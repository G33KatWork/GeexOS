#ifndef PLACEMENT_ALLOCATOR_H_
#define PLACEMENT_ALLOCATOR_H_

#include <types.h>
#include <arch/types.h>

namespace Memory
{
    Address GetPlacementBeginning();
    
    class PlacementAllocator
    {
    public:
        PlacementAllocator();
        
        void* Allocate(size_t len, bool pageAlign);
        unsigned int GetPointerPosition();
        size_t GetPlacementAllocatedSize();

    private:
        unsigned int placement_address;
    };
}
#endif
