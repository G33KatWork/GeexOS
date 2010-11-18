#ifndef PHYSICAL_MEMORY_MANAGER_H_
#define PHYSICAL_MEMORY_MANAGER_H_

#include <types.h>
#include <arch/types.h>
#include <kernel/DataStructures/Bitfield.h>

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
        DataStructures::Bitfield *frames;
        uint32_t nFrames;
    };
}
#endif
