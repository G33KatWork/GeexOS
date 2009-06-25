#ifndef PHYSICAL_MEMORY_MANAGER_H_
#define PHYSICAL_MEMORY_MANAGER_H_

#include <lib/types.h>
#include <kernel/Memory/IPhysicalMemoryManager.h>

namespace Memory
{
    class BitfieldPhysicalMemoryManager : public IPhysicalMemoryManager
    {
    public:
        BitfieldPhysicalMemoryManager(unsigned int memorySize);
        Address AllocateFrame();
        void DeallocateFrame(Address a);
        
    private:
        uint32_t *frames;
        uint32_t nFrames;
        
        unsigned int bitmap_get_first_free(void);
        void bitmap_set_frame(Address physAddr);
    };
}
#endif
