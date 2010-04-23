#ifndef PLACEMENT_ALLOCATOR_H_
#define PLACEMENT_ALLOCATOR_H_

#include <lib/types.h>

namespace Memory
{
    #define     PLACEMENT_SIZE      0x400000             //4MB
    
    Address GetPlacementBeginning();
    
    class PlacementAllocator
    {
    public:
        PlacementAllocator();
        
        void* Allocate(size_t len, bool pageAlign);
        unsigned int GetPointerPosition();

    private:
        unsigned int placement_address;
    };
}
#endif
