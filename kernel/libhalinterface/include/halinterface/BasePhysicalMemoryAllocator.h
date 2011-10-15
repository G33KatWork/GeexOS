#ifndef _HAL_INTERFACES_BASEPHYSICALMEMORYALLOCATOR_H
#define _HAL_INTERFACES_BASEPHYSICALMEMORYALLOCATOR_H

#include <types.h>
#include <halinterface/BaseOutputDevice.h>

namespace Arch
{
    class BasePhysicalMemoryAllocator
    {
    protected:
        BasePhysicalMemoryAllocator() {}
        virtual ~BasePhysicalMemoryAllocator() {}
    
    public:
        virtual Address AllocateFrame() = 0;
        virtual void DeallocateFrame(Address physAddr) = 0;
        virtual void MarkAsUsed(Address physAddr) = 0;
        virtual bool IsFree(Address physAddr) = 0;
        
        virtual void DumpUsed(Debug::BaseOutputDevice* c) = 0;
    };
}

#endif
