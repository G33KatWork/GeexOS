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
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Allocating a VirtualMemoryRegion " << regionName << " in VirtualMemorySpace " << name);
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Starting Address " << hex << address <<
                                   " Size: " << size <<
                                   " Flags: " << (flags & ALLOCFLAG_WRITABLE?"rw":"ro") <<
                                          " " << (flags & ALLOCFLAG_USERMODE?"umode":"kmode") << 
                                          " " << (flags & ALLOCFLAG_EXECUTABLE?"exec":"noexec"));
    
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

VirtualMemoryRegion* VirtualMemorySpace::AllocateInRange(Address startAddress, Address endAddress, size_t size, const char* regionName, AllocationFlags flags)
{
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Allocating a VirtualMemoryRegion " << regionName << " in VirtualMemorySpace " << name << " in a range");
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Starting Address of range: " << hex << startAddress <<
                                   " End Address of range: " << hex << endAddress <<
                                   " Size: " << size <<
                                   " Flags: " << (flags & ALLOCFLAG_WRITABLE?"rw":"ro") <<
                                          " " << (flags & ALLOCFLAG_USERMODE?"umode":"kmode") << 
                                          " " << (flags & ALLOCFLAG_EXECUTABLE?"exec":"noexec"));
    
    size_t continousFreeSpace = 0;
    
    for(Address curAddr = startAddress; curAddr < endAddress; curAddr += PAGE_SIZE)
    {
        if(continousFreeSpace >= size)
        {
            VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Seems we found a hole large enough to fit our request at " << hex << (curAddr - size));
            return Allocate(curAddr-size, size, regionName, flags);
        }
        
        if(FindRegionEnclosingAddress(curAddr) == NULL)
            continousFreeSpace += PAGE_SIZE;
        else
            continousFreeSpace = 0;
    }
    
    return NULL;
}

void VirtualMemorySpace::Deallocate(VirtualMemoryRegion* region)
{
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Deallocating region " << region->name << " in VirtualMemorySpace " << this->name);
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Starting Address " << hex << region->startAddress <<
                                   " Size: " << region->size <<
                                   " Flags: " << (region->flags & ALLOCFLAG_WRITABLE?"rw":"ro") <<
                                          " " << (region->flags & ALLOCFLAG_USERMODE?"umode":"kmode") << 
                                          " " << (region->flags & ALLOCFLAG_EXECUTABLE?"exec":"noexec"));
    
    ASSERT(region != NULL, "Supplied region is NULL");
    if(region == NULL)
        return;
    
    for(Address i = region->StartAddress(); i < region->StartAddress() + region->Size(); i+=PAGE_SIZE)
        Paging::GetInstance()->UnmapAddress(i);
    
    RemoveRegion(region);
    delete region;
}

/*VirtualMemoryRegion* VirtualMemorySpace::MapPhysical(Address physAddr, Address virtAddr, size_t size, const char* regionName, AllocationFlags flags)
{
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Mapping a physical address to a VirtualMemoryRegion " << regionName << " in VirtualMemorySpace " << name);
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Physical Address " << hex << physAddr <<
                                   " Virtual Address " << hex << virtAddr <<
                                   " Size: " << size <<
                                   " Flags: " << (flags & ALLOCFLAG_WRITABLE?"rw":"ro") <<
                                          " " << (flags & ALLOCFLAG_USERMODE?"umode":"kmode") << 
                                          " " << (flags & ALLOCFLAG_EXECUTABLE?"exec":"noexec"));
    
    VirtualMemoryRegion* region = new VirtualMemoryRegion(virtAddr, size, regionName);
    
    for(Address i = region->startAddress; i < region->startAddress + region->size; i += PAGE_SIZE)
    {
        ASSERT(!this->manager->PhysicalAllocator()->IsFree(physAddr), "Physical frame to be mapped manually into a virtual memory address is not marked as used");
        ASSERT(!Paging::GetInstance()->IsPresent(i), "Virtual address which should be mapped manually to a physical frame is already present");
        
        Paging::GetInstance()->MapAddress(
            i,
            physAddr,
            flags & ALLOCFLAG_WRITABLE,
            flags & ALLOCFLAG_USERMODE
        );
        //this->manager->PhysicalAllocator()->MarkAsUsed(physAddr);
        
        physAddr += PAGE_SIZE;
    }
    
    AddRegion(region);
    return region;
}*/

void VirtualMemorySpace::SetFlags(VirtualMemoryRegion* r, AllocationFlags f)
{
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Correcting alloc flags for VirtualMemoryRegion " << r->Name() << " in VirtualMemorySpace " << this->name);
    
    r->flags = f;
    
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Starting Address " << hex << r->startAddress <<
                                   " Size: " << r->size <<
                                   " Flags: " << (f & ALLOCFLAG_WRITABLE?"rw":"ro") <<
                                          " " << (f & ALLOCFLAG_USERMODE?"umode":"kmode") << 
                                          " " << (f & ALLOCFLAG_EXECUTABLE?"exec":"noexec"));
    
    for(Address i = r->startAddress; i < r->startAddress + r->size; i += PAGE_SIZE)
    {
        VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Remapping page at virtual " << hex << i);
        Paging::GetInstance()->MapAddress(
            i,
            Paging::GetInstance()->GetPhysicalAddress(i),
            f & ALLOCFLAG_WRITABLE,
            f & ALLOCFLAG_USERMODE
        );
    }
}

void VirtualMemorySpace::AnnounceRegion(Address address, size_t size, const char* rname, AllocationFlags f)
{
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Announcing region " << rname << " in VirtualMemorySpace " << this->name);
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Starting Address " << hex << address <<
                                   " Size: " << size <<
                                   " Flags: " << (f & ALLOCFLAG_WRITABLE?"rw":"ro") <<
                                          " " << (f & ALLOCFLAG_USERMODE?"umode":"kmode") << 
                                          " " << (f & ALLOCFLAG_EXECUTABLE?"exec":"noexec"));
    
    VirtualMemoryRegion* region = new VirtualMemoryRegion(address, size, rname);
    SetFlags(region, f);
    AddRegion(region);
}

void VirtualMemorySpace::Remap(VirtualMemoryRegion* region, Address NewAddress)
{
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Remapping region " << region->name << " in VirtualMemorySpace " << this->name);
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("OLD: Starting Address " << hex << region->startAddress <<
                                   " Size: " << region->size <<
                                   " Flags: " << (region->flags & ALLOCFLAG_WRITABLE?"rw":"ro") <<
                                          " " << (region->flags & ALLOCFLAG_USERMODE?"umode":"kmode") << 
                                          " " << (region->flags & ALLOCFLAG_EXECUTABLE?"exec":"noexec"));
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("NEW: Starting Address " << hex << NewAddress);
    
    ASSERT(region != NULL, "Supplied region is NULL");
    if(region == NULL)
        return;
    
    Address newStartAddress = NewAddress;
    
    for(Address i = region->StartAddress(); i < region->StartAddress() + region->Size(); i+=PAGE_SIZE)
    {
        Paging::GetInstance()->MapAddress(
            NewAddress,
            Paging::GetInstance()->GetPhysicalAddress(i),
            region->flags & ALLOCFLAG_WRITABLE,
            region->flags & ALLOCFLAG_USERMODE
        );
        Paging::GetInstance()->UnmapAddress(i);
        NewAddress += PAGE_SIZE;
    }
    
    region->startAddress = newStartAddress;
}

bool VirtualMemorySpace::SwapOutRegion(VirtualMemoryRegion* UNUSED(r), Address UNUSED(address), size_t UNUSED(s))
{
    return false;
}

bool VirtualMemorySpace::SwapInRegion(VirtualMemoryRegion* UNUSED(r), Address UNUSED(address), size_t UNUSED(s))
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

VirtualMemoryRegion* VirtualMemorySpace::FindRegionEnclosingAddress(Address addr)
{
    for(VirtualMemoryRegion* curRegion = RegionListHead; curRegion != NULL; curRegion = curRegion->Next)
    {
        if(addr >= curRegion->startAddress && addr < (curRegion->startAddress + curRegion->size))
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
