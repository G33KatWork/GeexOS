#ifndef VIRTUALMEMORYMANAGER_H_
#define VIRTUALMEMORYMANAGER_H_

#include <lib/types.h>
#include <kernel/Memory/BitfieldPhysicalMemoryManager.h>
#include <kernel/Memory/Virtual/VirtualMemorySpace.h>

namespace Memory
{
    class VirtualMemoryManager
    {
    public:
        VirtualMemoryManager(size_t MemorySize);
        
        void AddSpace(VirtualMemorySpace* space);
        void RemoveSpace(VirtualMemorySpace* space);
        
        VirtualMemorySpace* FindSpaceByName(const char* spaceName);
        
        VirtualMemorySpace* KernelSpace() { return kernelSpace; }
        void KernelSpace(VirtualMemorySpace* NewKernelSpace) { kernelSpace = NewKernelSpace; }
        
        BitfieldPhysicalMemoryManager* PhysicalAllocator() { return phys; }
        
    private:
        BitfieldPhysicalMemoryManager* phys;
        
        VirtualMemorySpace* SpaceListHead;
        VirtualMemorySpace* kernelSpace;
    };
}
#endif
