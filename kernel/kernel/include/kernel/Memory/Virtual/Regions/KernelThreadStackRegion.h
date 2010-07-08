#ifndef KERNELTHREADSTACKMEMORYREGION_H_
#define KERNELTHREADSTACKMEMORYREGION_H_

#include <types.h>
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
        Address beginning;
        size_t curSize;
        size_t maxSize;
        
        KernelThreadStack* next;
        
        KernelThreadStack(Address stackBeginning, size_t CurSize, size_t MaxSize, KernelThreadStack* Next)
        {
            beginning = stackBeginning;
            curSize = CurSize;
            maxSize = MaxSize;
            next = Next;
        }
    
    public:
        Address Beginning() { return beginning; }
    };
    
    /*  This region maintains all the stacks of kernel threads
     */
    class KernelThreadStackMemoryRegion : public LazyMemoryRegion
    {
        friend class VirtualMemorySpace;
    private:
        size_t initialSize;
        size_t curSize;
        
        KernelThreadStack* stackList;
    
    public:
        KernelThreadStackMemoryRegion(Address RegionStart, size_t MaxRegionSize, const char* RegionName)
            : LazyMemoryRegion(RegionStart, MaxRegionSize, RegionName, ALLOCFLAG_WRITABLE)
        {
            stackList = NULL;
        }
        
        KernelThreadStack* CreateStack(size_t InitialSize, size_t MaxSize);
        void DestroyStack(KernelThreadStack* stack);
        
        virtual bool HandlePageFault();
		
        void DumpStacks(IO::BaseDebugOutputDevice* c);
		
	private:
        void AddToList(KernelThreadStack* toAdd);
        void RemoveFromList(KernelThreadStack* toRemove);
    };
}
#endif
