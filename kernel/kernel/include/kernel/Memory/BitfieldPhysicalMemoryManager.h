#ifndef PHYSICAL_MEMORY_MANAGER_H_
#define PHYSICAL_MEMORY_MANAGER_H_

#include <types.h>

namespace Memory
{
    class BitfieldPhysicalMemoryManager
    {
    public:
        BitfieldPhysicalMemoryManager(size_t memorySize);
        Address AllocateFrame();
        void DeallocateFrame(Address physAddr);
        void MarkAsUsed(Address physAddr);
        bool IsFree(Address physAddr);
        
    private:
        uint32_t *frames;
        uint32_t nFrames;
        
        unsigned int bitmap_get_first_free(void);
        void bitmap_set_frame(Address physAddr);
        void bitmap_clear_frame(Address physAddr);
    };
}
#endif
