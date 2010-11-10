#ifndef KERNELTHREADSTACKMEMORYREGION_H_
#define KERNELTHREADSTACKMEMORYREGION_H_

#include <types.h>
#include <arch/types.h>
#include <kernel/global.h>
#include <kernel/Memory/Virtual/Regions/LazyMemoryRegion.h>
#include <kernel/debug.h>

namespace Memory
{
    /* This class represents a stack of a kernel thread
     * They are only meant to be created by KernelThreadStackMemoryRegion
     */
    class KernelThreadStack
    {
        friend class KernelThreadStackMemoryRegion;
    private:
        Address beginning;	/* LOWERMOST address */
        size_t stackSize;
        
        KernelThreadStack* next;
        
        KernelThreadStack(Address stackBeginning, size_t StackSize, KernelThreadStack* Next)
        {
            beginning = stackBeginning;
            stackSize = StackSize;
            next = Next;
        }
    
    public:
        Address GetEndAddress() { return beginning; }				//lowermost address
		Address GetStartAddress() { return beginning + stackSize; }	//uppermost address
		size_t GetSize() { return stackSize; }
    };
    
    /*  This region maintains all the stacks of kernel threads
     */
    class KernelThreadStackMemoryRegion : public VirtualMemoryRegion
    {
        friend class VirtualMemorySpace;
    private:
        size_t initialSize;
        size_t curSize;
        
        KernelThreadStack* stackList;
    
    public:
        KernelThreadStackMemoryRegion(Address RegionStart, size_t MaxRegionSize, const char* RegionName)
            : VirtualMemoryRegion(RegionStart, MaxRegionSize, RegionName, ALLOCFLAG_WRITABLE)
        {
            stackList = NULL;
        }
        
        KernelThreadStack* CreateStack(size_t StackSize);
        void DestroyStack(KernelThreadStack* stack);
        
        virtual bool HandlePageFault(Address faultingAddress);
		
        void DumpStacks(Debug::BaseDebugOutputDevice* c);
		
	private:
        void AddToList(KernelThreadStack* toAdd);
        void RemoveFromList(KernelThreadStack* toRemove);
    };
}
#endif
