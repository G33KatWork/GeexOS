#ifndef VIRTUALMEMORYSPACE_H_
#define VIRTUALMEMORYSPACE_H_

#include <lib/types.h>
#include <kernel/Memory/Virtual/VirtualMemoryRegion.h>

#include <kernel/IO/CharacterOutputDevice.h>

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
		
		void AddRegion(VirtualMemoryRegion* region);
		void RemoveRegion(VirtualMemoryRegion* region);
		
		VirtualMemoryRegion* Allocate(Address address, size_t size, const char* regionName, AllocationFlags flags);
        void Deallocate(VirtualMemoryRegion* region);
		
		VirtualMemoryRegion* FindRegionByName(const char* regionName);
		VirtualMemoryRegion* FindRegionByStartAddress(Address start);
		
		AllocationFlags GetFlags(VirtualMemoryRegion* r) { return r->flags; }
        void SetFlags(VirtualMemoryRegion* r, AllocationFlags f);
        
        void Remap(VirtualMemoryRegion* r, Address NewAddress);
        
        bool SwapOutRegion(VirtualMemoryRegion* r, Address address, size_t s);
        bool SwapInRegion(VirtualMemoryRegion* r, Address address, size_t s);
		
		void DumpRegions(IO::CharacterOutputDevice& c);
		
        const char* Name() { return name; }
		
	private:
	    VirtualMemoryManager* manager;
        const char* name;
        
		//FIXME: For performance reasons we do not really want to use a list here. AVL-Tree? Red-Black-Tree? Anyone?
		VirtualMemoryRegion* RegionListHead;
		
		VirtualMemorySpace *Next;
	};
}
#endif
