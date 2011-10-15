#ifndef BUDDYALLOCATEDMEMORYREGION_H_
#define BUDDYALLOCATEDMEMORYREGION_H_

#include <types.h>
#include <kernel/Memory/Virtual/Regions/LazyMemoryRegion.h>
#include <kernel/DataStructures/Bitfield.h>

//TODO: Implement method to return all unneeded physical memory to the physical allocator

namespace Memory
{
    struct BuddyElement
    {
        size_t order;
        struct BuddyElement* next;
    };
    
    class BuddyAllocatedMemoryRegion : public LazyMemoryRegion
    {
        friend class VirtualMemorySpace;
        
    private:
		struct BuddyElement** freeElements;
        DataStructures::Bitfield *elementsAllocated;    //0 = free, 1 = allocated
        size_t maxOrder;
        size_t minOrder;
		
        bool IsPowerOf2(size_t n) { return !(n & (n-1)); }
        uint32_t GetBitmapIndex(Address buddyStart) { return (buddyStart - this->startAddress) >> PAGE_SHIFT; }
        void MarkElementFree(Address buddyStart) { elementsAllocated->ClearBit(GetBitmapIndex(buddyStart)); }
        void MarkElementAllocated(Address buddyStart) { elementsAllocated->SetBit(GetBitmapIndex(buddyStart)); }
        bool IsElementFree(Address buddyStart) { return !elementsAllocated->IsBitSet(GetBitmapIndex(buddyStart)); }
        void RemoveFromList(struct BuddyElement** listHead, struct BuddyElement* toRemove);

    public:
        BuddyAllocatedMemoryRegion(Address RegionStart, size_t RegionSize, const char* RegionName, size_t MinimumOrder);
        virtual ~BuddyAllocatedMemoryRegion() {}
        
        virtual bool HandlePageFault(Address faultingAddress);
		
        Address AllocateBuddy(size_t order);
        void FreeBuddy(Address addr, size_t order);
        void DumpBuddyInfo(Debug::BaseOutputDevice* c);
    };
}
#endif
