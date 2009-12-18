#include <kernel/Memory/VirtualMemorySpace.h>
#include <lib/string.h>
#include <kernel/global.h>

using namespace Memory;
using namespace IO;

void VirtualMemorySpace::AddRegion(VirtualMemoryRegion* region)
{
    region->Next = ListHead;
    ListHead = region;
}

void VirtualMemorySpace::RemoveRegion(VirtualMemoryRegion* region)
{
    if(ListHead == region)
    {
        ListHead = region->Next;
        return;
    }
    
    VirtualMemoryRegion* curItem = ListHead;
    
    while(curItem->Next != region && curItem->Next != NULL)
        	curItem = curItem->Next;
    
    if(curItem->Next == region)
    {
        DEBUG_MSG("Removing item at " << hex << (unsigned)curItem->Next);
        curItem->Next = curItem->Next->Next;
    }
}

VirtualMemoryRegion* VirtualMemorySpace::FindRegionByName(const char* name)
{
    for(VirtualMemoryRegion* curRegion = ListHead; curRegion != NULL; curRegion = curRegion->Next)
    {
        if(!strcmp(curRegion->Name, name))
            return curRegion;
    }
    
    return NULL;
}

VirtualMemoryRegion* VirtualMemorySpace::FindRegionByStartAddress(Address start)
{
    for(VirtualMemoryRegion* curRegion = ListHead; curRegion != NULL; curRegion = curRegion->Next)
    {
        if(curRegion->StartAddress == start)
            return curRegion;
    }
    
    return NULL;
}

void VirtualMemorySpace::DumpRegions(CharacterOutputDevice& c)
{
    for(VirtualMemoryRegion* curRegion = ListHead; curRegion != NULL; curRegion = curRegion->Next)
    {
        c << "VMEMSPACE: " << "\tName: " << curRegion->Name << endl;
        c << "VMEMSPACE: " << "\tStartAddress: " << hex << (unsigned int)curRegion->StartAddress << endl;
        c << "VMEMSPACE: " << "\tSize: " << hex << (unsigned int)curRegion->Size << endl;
        c << "VMEMSPACE: " << "\tAllocator: " << hex << (unsigned int)curRegion->Allocator << endl;
        c << endl;
    }
}
