#ifndef _ARCHX86_PHYSICAL_MEMORY_ALLOCATOR_H_
#define _ARCHX86_PHYSICAL_MEMORY_ALLOCATOR_H_

#include <types.h>
#include <kernel/DataStructures/Bitfield.h>
#include <halinterface/BasePhysicalMemoryAllocator.h>

namespace Arch
{
    class BitfieldPhysicalMemoryAllocator : public BasePhysicalMemoryAllocator
    {
    public:
        BitfieldPhysicalMemoryAllocator(size_t memorySize);
        
        virtual Address AllocateFrame();
        virtual void DeallocateFrame(Address physAddr);
        virtual void MarkAsUsed(Address physAddr);
        virtual bool IsFree(Address physAddr);
        
    private:
        DataStructures::Bitfield *frames;
        uint32_t nFrames;
    };
}
#endif
