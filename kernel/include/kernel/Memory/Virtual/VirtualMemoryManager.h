#ifndef VIRTUALMEMORYMANAGER_H_
#define VIRTUALMEMORYMANAGER_H_

#include <lib/types.h>
#include <kernel/Memory/BitfieldPhysicalMemoryManager.h>
#include <kernel/Memory/Virtual/VirtualMemorySpace.h>
#include <kernel/Memory/Stack.h>

namespace Memory
{
    class VirtualMemoryManager
    {
    public:
        static VirtualMemoryManager* GetInstance();
        
        void Init(size_t MemorySize);
        
        void AddSpace(VirtualMemorySpace* space);
        void RemoveSpace(VirtualMemorySpace* space);
        
        VirtualMemorySpace* FindSpaceByName(const char* spaceName);
        
        VirtualMemorySpace* KernelSpace() { return kernelSpace; }
        void KernelSpace(VirtualMemorySpace* NewKernelSpace) { kernelSpace = NewKernelSpace; }
        Stack* KernelStack() { return kernelStack; }
        void KernelStack(Stack* NewKernelStack) { kernelStack = NewKernelStack; }
        
        BitfieldPhysicalMemoryManager* PhysicalAllocator() { return phys; }
        
    private:
        static VirtualMemoryManager* instance;
        
        BitfieldPhysicalMemoryManager* phys;
        
        VirtualMemorySpace* SpaceListHead;
        
        VirtualMemorySpace* kernelSpace;
        Stack* kernelStack;
    };
}
#endif
