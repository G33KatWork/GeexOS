#ifndef PLACEMENT_ALLOCATOR_H_
#define PLACEMENT_ALLOCATOR_H_

#include <lib/types.h>

namespace Memory
{
    #define     PLACEMENT_SIZE      0x20000             //128KByte
    
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
