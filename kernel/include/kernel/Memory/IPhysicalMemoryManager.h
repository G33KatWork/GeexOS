#include <lib/types.h>

#ifndef IPHYSICAL_MEMORY_MANAGER_H
#define IPHYSICAL_MEMORY_MANAGER_H

namespace Memory
{
    //Interface for physical memory managers
    class IPhysicalMemoryManager
    {
    protected:
        virtual ~IPhysicalMemoryManager() {};

    public:    
        virtual Address AllocateFrame() = 0;
        virtual void DeallocateFrame(Address a) = 0;
    };
}

#endif
