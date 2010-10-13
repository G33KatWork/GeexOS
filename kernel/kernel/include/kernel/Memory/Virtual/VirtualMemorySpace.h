#ifndef VIRTUALMEMORYSPACE_H_
#define VIRTUALMEMORYSPACE_H_

#include <types.h>
#include <arch/types.h>
#include <kernel/Memory/Virtual/VirtualMemoryRegion.h>
#include <halinterface/BaseDebugOutputDevice.h>

namespace Memory
{
    class VirtualMemoryManager;
    
	/*
	 *	This class holds several memory regions, which form the virtual
	 *	address space
	 */
	class VirtualMemorySpace
	{
    friend class VirtualMemoryManager;
        
	public:
        VirtualMemorySpace(VirtualMemoryManager* ParentManager, const char* SpaceName)
        {
            RegionListHead = NULL;
            Next = NULL;
            name = SpaceName;
            manager = ParentManager;
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
		
		void DumpRegions(Debug::BaseDebugOutputDevice* c);
		
        const char* Name() { return name; }
		
	private:
	    VirtualMemoryManager* manager;
        const char* name;
        
		//FIXME: For performance reasons we do not really want to use a list here. AVL-Tree? Red-Black-Tree? Anyone?
		VirtualMemoryRegion* RegionListHead;
		
		VirtualMemorySpace *Next;
		
		void AddRegionToList(VirtualMemoryRegion* region);
		void RemoveRegionFromList(VirtualMemoryRegion* region);
	};
}
#endif