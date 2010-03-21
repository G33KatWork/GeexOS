#include <kernel/Memory/Virtual/VirtualMemorySpace.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <lib/string.h>
#include <kernel/global.h>
#include <kernel/debug.h>
#include <arch/Paging.h>

using namespace Memory;
using namespace IO;
using namespace Arch;

void VirtualMemorySpace::AddRegion(VirtualMemoryRegion* region)
{
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Adding VirtualMemoryRegion " << region->name << " to VirtualMemorySpace " << name);
    
    region->Next = RegionListHead;
    RegionListHead = region;
}

void VirtualMemorySpace::RemoveRegion(VirtualMemoryRegion* region)
{
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Removing VirtualMemoryRegion " << region->name << " from VirtualMemorySpace" << name);
    
    if(RegionListHead == region)
    {
        RegionListHead = region->Next;
        return;
    }
    
    VirtualMemoryRegion* curItem = RegionListHead;
    
    while(curItem->Next != region && curItem->Next != NULL)
        	curItem = curItem->Next;
    
    if(curItem->Next == region)
        curItem->Next = curItem->Next->Next;
}

VirtualMemoryRegion* VirtualMemorySpace::Allocate(Address address, size_t size, const char* regionName, AllocationFlags flags)
{
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Allocating a VirtualMemoryRegion " << regionName << " in VirtualMemorySpace" << name);
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Starting Address " << (unsigned)address <<
                                   " Size: " << (unsigned)size <<
                                   " Flags: " << (flags & ALLOCFLAG_WRITABLE?"rw":"ro") <<
                                          " " << (flags & ALLOCFLAG_USERMODE?"umode":"kmode") << 
                                          " " << (flags & ALLOCFLAG_EXECUTABLE?"exec":"noexec"));
    
    ASSERT(IS_PAGE_ALIGNED(address), "Starting address of a region must be page aligned.");
    ASSERT(size % PAGE_SIZE == 0, "Size of a region must be a multiple of the page size.");
    
    VirtualMemoryRegion* r = new VirtualMemoryRegion(address, size, regionName);
    r->flags = flags;
    
    for(Address i = address; i < address + size; i+=PAGE_SIZE)
    {
        Paging::GetInstance()->MapAddress(
            i,
            this->manager->PhysicalAllocator()->AllocateFrame(),
            flags & ALLOCFLAG_WRITABLE,
            flags & ALLOCFLAG_USERMODE
        );
    }
    
    AddRegion(r);
    
    return r;
}

void VirtualMemorySpace::Deallocate(VirtualMemoryRegion* region)
{
    
}

void VirtualMemorySpace::SetFlags(VirtualMemoryRegion* r, AllocationFlags f)
{
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Correcting alloc flags for " << Name);
    
    r->flags = f;
    
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Starting address: " << hex << (unsigned)r->startAddress);
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Size: " << hex << (unsigned)r->size);
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("New flags: " << (f & ALLOCFLAG_WRITABLE?"rw":"ro") <<
                                    " " << (f & ALLOCFLAG_USERMODE?"umode":"kmode") << 
                                    " " << (f & ALLOCFLAG_EXECUTABLE?"exec":"noexec"));
    
    for(Address i = r->startAddress; i < PAGE_ALIGN(r->startAddress + r->size); i += PAGE_SIZE)
    {
        Paging::GetInstance()->MapAddress(
            i,
            Paging::GetInstance()->GetPhysicalAddress(i),
            f & ALLOCFLAG_WRITABLE,
            f & ALLOCFLAG_USERMODE
        );
    }
}

void Remap(VirtualMemoryRegion* r, Address NewAddress)
{
    
}

bool VirtualMemorySpace::SwapOutRegion(VirtualMemoryRegion* r, Address address, size_t s)
{
    return false;
}

bool VirtualMemorySpace::SwapInRegion(VirtualMemoryRegion* r, Address address, size_t s)
{
    return false;
}

VirtualMemoryRegion* VirtualMemorySpace::FindRegionByName(const char* regionName)
{
    for(VirtualMemoryRegion* curRegion = RegionListHead; curRegion != NULL; curRegion = curRegion->Next)
    {
        if(!strcmp(curRegion->Name(), regionName))
            return curRegion;
    }
    
    return NULL;
}

VirtualMemoryRegion* VirtualMemorySpace::FindRegionByStartAddress(Address start)
{
    for(VirtualMemoryRegion* curRegion = RegionListHead; curRegion != NULL; curRegion = curRegion->Next)
    {
        if(curRegion->startAddress == start)
            return curRegion;
    }
    
    return NULL;
}

void VirtualMemorySpace::DumpRegions(CharacterOutputDevice& c)
{
    for(VirtualMemoryRegion* curRegion = RegionListHead; curRegion != NULL; curRegion = curRegion->Next)
    {
        c << "VMEMSPACE: " << "\tName: " << curRegion->name << endl;
        c << "VMEMSPACE: " << "\tStartAddress: " << hex << (unsigned int)curRegion->startAddress << endl;
        c << "VMEMSPACE: " << "\tSize: " << hex << (unsigned int)curRegion->size << endl;
        c << endl;
    }
}
