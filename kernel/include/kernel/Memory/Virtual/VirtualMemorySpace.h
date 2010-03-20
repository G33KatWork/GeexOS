#ifndef VIRTUALMEMORYSPACE_H_
#define VIRTUALMEMORYSPACE_H_

#include <lib/types.h>
#include <kernel/Memory/Virtual/VirtualMemoryRegion.h>

#include <kernel/IO/CharacterOutputDevice.h>

namespace Memory
{
	/*
	 *	This class holds several memory regions, which form the virtual
	 *	address space
	 */
	class VirtualMemorySpace
	{
	public:
		VirtualMemorySpace(){ListHead = NULL;}
		
		void AddRegion(VirtualMemoryRegion* region);
		void RemoveRegion(VirtualMemoryRegion* region);
		
		VirtualMemoryRegion* Allocate(Address address, size_t size, const char* name, AllocationFlags flags);
        void Deallocate(VirtualMemoryRegion* region);
		
		VirtualMemoryRegion* FindRegionByName(const char* name);
		VirtualMemoryRegion* FindRegionByStartAddress(Address start);
		
		void DumpRegions(IO::CharacterOutputDevice& c);
		
	private:
		//FIXME: For performance reasons we do not really want to use a list here. AVL-Tree? Red-Black-Tree? Anyone?
		VirtualMemoryRegion* ListHead;
	};
}
#endif
