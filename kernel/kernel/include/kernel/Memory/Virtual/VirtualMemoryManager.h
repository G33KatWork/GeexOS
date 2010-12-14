#ifndef VIRTUALMEMORYMANAGER_H_
#define VIRTUALMEMORYMANAGER_H_

#include <types.h>
#include <kernel/Memory/BitfieldPhysicalMemoryManager.h>
#include <kernel/Memory/Virtual/VirtualMemorySpace.h>
#include <kernel/Memory/Virtual/Regions/KernelStackMemoryRegion.h>
#include <kernel/utils/ElfInformation.h>
#include <kernel/Memory/Virtual/Regions/KernelThreadStackRegion.h>
#include <kernel/Memory/Slab/SlabAllocator.h>
#include <kernel/Memory/IO/IOMemoryManager.h>
#include <kernel/DataStructures/DoublyLinkedList.h>

namespace Memory
{
    class IOMemoryManager;      //Avoid ciruclar header inclusion
    
    class VirtualMemoryManager
    {
    public:
        static VirtualMemoryManager* GetInstance();
        
        VirtualMemoryManager();
        
        void Init(size_t MemorySize);
        
        void AddSpace(VirtualMemorySpace* space);
        void RemoveSpace(VirtualMemorySpace* space);
        
        VirtualMemorySpace* FindSpaceByName(const char* spaceName);
        
        /* Sets the current memory space which is used. Also changes the associated page directory! */
        void SetCurrentMemorySpace(VirtualMemorySpace* newSpace);
        VirtualMemorySpace* GetCurrentMemorySpace() { return currentSpace; }
        
        BitfieldPhysicalMemoryManager* PhysicalAllocator() { return phys; }
        IOMemoryManager* IOMemory() { return iomanager; }
        
        VirtualMemorySpace* KernelSpace() { return kernelSpace; }
        void KernelSpace(VirtualMemorySpace* NewKernelSpace) { kernelSpace = NewKernelSpace; }
        KernelStackMemoryRegion* KernelStack() { return kernelStack; }
        void KernelStack(KernelStackMemoryRegion* NewKernelStack) { kernelStack = NewKernelStack; }
        ElfInformation* KernelElf() { return kernelElf; }
        void KernelElf(ElfInformation* NewKernelElf) { kernelElf = NewKernelElf; }
        KernelThreadStackMemoryRegion* KernelThreadStacks() { return kernelThreadStacks; }
        void KernelThreadStacks(KernelThreadStackMemoryRegion* NewKernelThreadStacks) { kernelThreadStacks = NewKernelThreadStacks; }
        Slab::SlabAllocator* SlabAllocator() { return slabAllocator; }
        void SlabAllocator(Slab::SlabAllocator* NewSlabAllocator) { slabAllocator = NewSlabAllocator; }
        
    private:
        static VirtualMemoryManager instance;
        
        BitfieldPhysicalMemoryManager* phys;
        
        //All important regions which are often needed
        IOMemoryManager* iomanager;
        VirtualMemorySpace* currentSpace;
        VirtualMemorySpace* kernelSpace;
        KernelStackMemoryRegion* kernelStack;
        ElfInformation* kernelElf;
        KernelThreadStackMemoryRegion* kernelThreadStacks;
        Slab::SlabAllocator* slabAllocator;
        
        DataStructures::DoublyLinkedList<VirtualMemorySpace> memorySpaceList;
    };
}
#endif
