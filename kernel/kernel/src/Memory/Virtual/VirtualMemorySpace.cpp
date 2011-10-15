#include <kernel/Memory/Virtual/VirtualMemorySpace.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
//#include <kernel/Memory/Virtual/Regions/PreallocatedMemoryRegion.h>
#include <string.h>
#include <kernel/global.h>
#include <kernel/debug.h>
#include <arch/HAL.h>

using namespace Memory;
using namespace Debug;
using namespace Arch;

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
        if(CurrentHAL->GetPaging()->IsPresent(i))
            CurrentHAL->GetPaging()->MapAddress(
                i,
                CurrentHAL->GetPaging()->GetPhysicalAddress(i),
                f & ALLOCFLAG_WRITABLE,
                f & ALLOCFLAG_USERMODE
            );
        else
        {
            VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Address " << hex << i << " wasn't present before, not remapping...");
        }
    }
}

void VirtualMemorySpace::AddRegion(VirtualMemoryRegion* region)
{
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Adding region " << region->name << " in VirtualMemorySpace " << this->name);
    //FIXME: check if space already occupied?
    AddRegionToList(region);
}

void VirtualMemorySpace::RemoveRegion(VirtualMemoryRegion* region)
{
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("removing region " << region->name << " in VirtualMemorySpace " << this->name);
    RemoveRegionFromList(region);
}

/*void VirtualMemorySpace::AnnounceRegionWithPreallocatedMemory(Address address, size_t size, const char* rname, AllocationFlags f)
{
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Announcing prealloc-region " << rname << " in VirtualMemorySpace " << this->name);
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Starting Address " << hex << address <<
                                   " Size: " << size <<
                                   " Flags: " << (f & ALLOCFLAG_WRITABLE?"rw":"ro") <<
                                          " " << (f & ALLOCFLAG_USERMODE?"umode":"kmode") << 
                                          " " << (f & ALLOCFLAG_EXECUTABLE?"exec":"noexec"));
    
    //FIXME: check if space already occupied?
    VirtualMemoryRegion* region = new PreallocatedMemoryRegion(address, size, rname, f);
    SetFlags(region, f);
    AddRegionToList(region);
}*/

/*void VirtualMemorySpace::Remap(VirtualMemoryRegion* region, Address NewAddress)
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
}*/

VirtualMemoryRegion* VirtualMemorySpace::FindRegionByName(const char* regionName)
{
    for(VirtualMemoryRegion* curRegion = memoryRegionList.Head(); curRegion != NULL; curRegion = memoryRegionList.GetNext(curRegion))
    {
        if(!strcmp(curRegion->Name(), regionName))
            return curRegion;
    }
    
    return NULL;
}

VirtualMemoryRegion* VirtualMemorySpace::FindRegionByStartAddress(Address start)
{
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("VirtualMemoryRegion at address " << hex << start << " in VirtualMemorySpace " << name << " was requested");

    for(VirtualMemoryRegion* curRegion = memoryRegionList.Head(); curRegion != NULL; curRegion = memoryRegionList.GetNext(curRegion))
    {
        if(curRegion->startAddress == start)
        {
            VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Found region " << curRegion->name);
            return curRegion;
        }
    }
    
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Requested region doesn't seem to exist in VirtualMemorySpace " << name);
    return NULL;
}    

VirtualMemoryRegion* VirtualMemorySpace::FindRegionEnclosingAddress(Address addr)
{
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("VirtualMemoryRegion enclosing address " << hex << addr << " in VirtualMemorySpace " << name << " was requested");
    
    for(VirtualMemoryRegion* curRegion = memoryRegionList.Head(); curRegion != NULL; curRegion = memoryRegionList.GetNext(curRegion))
    {
        if(addr >= curRegion->startAddress && addr < (curRegion->startAddress + curRegion->size))
        {
            VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Found region " << curRegion->name);
            return curRegion;
        }
    }
    
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Requested region doesn't seem to exist in VirtualMemorySpace " << name);
    return NULL;
}

void VirtualMemorySpace::DumpRegions(BaseOutputDevice* c)
{
    for(VirtualMemoryRegion* curRegion = memoryRegionList.Head(); curRegion != NULL; curRegion = memoryRegionList.GetNext(curRegion))
    {
        *c << "VMEMSPACE: " << "\tName: " << curRegion->name << endl;
        *c << "VMEMSPACE: " << "\tStartAddress: " << hex << (unsigned int)curRegion->startAddress << endl;
        *c << "VMEMSPACE: " << "\tSize: " << hex << (unsigned int)curRegion->size << endl;
        *c << endl;
    }
}

void VirtualMemorySpace::AddRegionToList(VirtualMemoryRegion* region)
{
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Adding VirtualMemoryRegion " << region->name << " to VirtualMemorySpace " << name);
    
    memoryRegionList.Append(region);
}

void VirtualMemorySpace::RemoveRegionFromList(VirtualMemoryRegion* region)
{
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Removing VirtualMemoryRegion " << region->name << " from VirtualMemorySpace" << name);
    
    memoryRegionList.Remove(region);
}


bool VirtualMemorySpace::HandlePageFault(Address faultingAddress)
{
    VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Handling pagefault in VirtualMemorySpace " << name << " at Address " << faultingAddress);
    
    VirtualMemoryRegion* faultingRegion = FindRegionEnclosingAddress(faultingAddress);
    if(!faultingRegion)
    {
        VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Given faulting address doesn't seem to belong to any region. Can't resolve!");
        return false;
    }
    else
    {
        VIRTUAL_MEMORY_SPACE_DEBUG_MSG("Given faulting address seems to belong to region " << faultingRegion->name << ". Handing over to region...");
        return faultingRegion->HandlePageFault(faultingAddress);
    }
}
