#ifndef VIRTUALMEMORYMANAGER_H_
#define VIRTUALMEMORYMANAGER_H_

#include <types.h>
#include <kernel/Memory/BitfieldPhysicalMemoryManager.h>
#include <kernel/Memory/Virtual/VirtualMemorySpace.h>
#include <kernel/Memory/Virtual/Regions/KernelStackMemoryRegion.h>
#include <kernel/utils/ElfInformation.h>
#include <kernel/Memory/Virtual/Regions/KernelThreadStackRegion.h>

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
        
    private:
        static VirtualMemoryManager* instance;
        
        BitfieldPhysicalMemoryManager* phys;
        
        //All important regions which are often needed
        IOMemoryManager* iomanager;
        VirtualMemorySpace* kernelSpace;
        KernelStackMemoryRegion* kernelStack;
        ElfInformation* kernelElf;
        KernelThreadStackMemoryRegion* kernelThreadStacks;
        
        VirtualMemorySpace* SpaceListHead;
    };
}
#endif