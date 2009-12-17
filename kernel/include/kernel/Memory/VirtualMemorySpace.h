#ifndef VIRTUALMEMORYSPACE_H_
#define VIRTUALMEMORYSPACE_H_

#include <lib/types.h>
#include <kernel/Memory/VirtualMemoryRegion.h>

namespace Memory
{
    /*
     *  This class holds several memory regions, which form the virtual
     *  address space
     */
    class VirtualMemorySpace
    {
    public:
        VirtualMemorySpace(){};
        
        void AddRegion(VirtualMemoryRegion* region){}
        void RemoveRegion(VirtualMemoryRegion* region){}
        
    private:
        
    };
}
#endif
