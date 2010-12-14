#ifndef VIRTUALMEMORYSPACE_H_
#define VIRTUALMEMORYSPACE_H_

#include <types.h>
#include <arch/types.h>
#include <kernel/Memory/Virtual/VirtualMemoryRegion.h>
#include <halinterface/BaseDebugOutputDevice.h>
#include <halinterface/BasePaging.h>
#include <kernel/DataStructures/DoublyLinkedList.h>

namespace Memory
{
    class VirtualMemoryManager;
    
	/*
	 *	This class holds several memory regions, which form the virtual
	 *	address space
	 */
	class VirtualMemorySpace : public DataStructures::DoublyLinkedListLinkImpl<VirtualMemorySpace>
	{
    friend class VirtualMemoryManager;
        
	public:
        VirtualMemorySpace(VirtualMemoryManager* ParentManager, const char* SpaceName, Arch::BasePageDirectory* pageDir)
        {
            name = SpaceName;
            manager = ParentManager;
            pageDirectory = pageDir;
        }
		
        void AnnounceRegion(VirtualMemoryRegion* region);
        void DiscontinueRegion(VirtualMemoryRegion* region);
		void AnnounceRegionWithPreallocatedMemory(Address address, size_t size, const char* rname, AllocationFlags f);
		
		VirtualMemoryRegion* FindRegionByName(const char* regionName);
		VirtualMemoryRegion* FindRegionByStartAddress(Address start);
		VirtualMemoryRegion* FindRegionEnclosingAddress(Address addr);
		
		AllocationFlags GetFlags(VirtualMemoryRegion* r) { return r->flags; }
        void SetFlags(VirtualMemoryRegion* r, AllocationFlags f);
        
        //void Remap(VirtualMemoryRegion* region, Address NewAddress);
        
        /* To be called by a page fault handler */
        bool HandlePageFault(Address faultingAddress);
		
		void DumpRegions(Debug::BaseDebugOutputDevice* c);
		
        const char* Name() { return name; }
		
	private:
	    VirtualMemoryManager* manager;
        const char* name;
        
		//FIXME: For performance reasons we do not really want to use a list here. AVL-Tree? Red-Black-Tree? Anyone?
		DataStructures::DoublyLinkedList<VirtualMemoryRegion> memoryRegionList;
		
        Arch::BasePageDirectory* pageDirectory;
		
		void AddRegionToList(VirtualMemoryRegion* region);
		void RemoveRegionFromList(VirtualMemoryRegion* region);
	};
}
#endif
