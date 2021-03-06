#include <kernel/Memory/IO/IOMemoryManager.h>
#include <string.h>
#include <kernel/debug.h>
#include <kernel/global.h>
#include <arch/HAL.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>

using namespace Memory;
using namespace Debug;
using namespace Arch;

IOMemoryManager::IOMemoryManager(Address IOMemoryStart, size_t IOMemorySize)
{
    IO_MEMORY_MANAGER_DEBUG_MSG("Initializing IOMemoryManager. Virtual I/O Memory starting at " << hex << IOMemoryStart << " with size " << hex << IOMemorySize);
    
    iomemStart = IOMemoryStart;
    iomemSize = IOMemorySize;
}

void IOMemoryManager::AddRegion(IOMemoryRegion* region)
{
    IO_MEMORY_MANAGER_DEBUG_MSG("Adding IOMemoryRegion " << region->name);

    regionList.Append(region);
}

/*void IOMemoryManager::RemoveRegion(IOMemoryRegion* region)
{
    IO_MEMORY_MANAGER_DEBUG_MSG("Removing IOMemoryRegion " << region->name);

    regionList.Remove(region);
}*/

IOMemoryRegion* IOMemoryManager::FindRegionByName(const char* regionName)
{
    for(IOMemoryRegion* curRegion = regionList.Head(); curRegion != NULL; curRegion = regionList.GetNext(curRegion))
    {
        if(!strcmp(curRegion->Name(), regionName))
            return curRegion;
    }
    
    return NULL;
}

IOMemoryRegion* IOMemoryManager::FindRegionEnclosingPhysicalAddress(Address physicalAddress)
{
    for(IOMemoryRegion* curRegion = regionList.Head(); curRegion != NULL; curRegion = regionList.GetNext(curRegion))
    {
        if(physicalAddress >= curRegion->startAddressPhysical && physicalAddress < (curRegion->startAddressPhysical + curRegion->size))
            return curRegion;
    }
    
    return NULL;
}

IOMemoryRegion* IOMemoryManager::FindRegionEnclosingVirtualAddress(Address virtualAddress)
{
    for(IOMemoryRegion* curRegion = regionList.Head(); curRegion != NULL; curRegion = regionList.GetNext(curRegion))
    {
        if(virtualAddress >= curRegion->startAddress && virtualAddress < (curRegion->startAddress + curRegion->size))
            return curRegion;
    }
    
    return NULL;
}

IOMemoryRegion* IOMemoryManager::MapPhysical(Address physicalAddress, size_t size, const char* regionName)
{
    IO_MEMORY_MANAGER_DEBUG_MSG("Mapping a physical I/O Memory range into I/O memory");
    IO_MEMORY_MANAGER_DEBUG_MSG("Physical Address " << hex << physicalAddress << ", size " << size
                                << " Name: " << regionName);
    
    ASSERT(IS_PAGE_ALIGNED(physicalAddress), "Physical address of IO memory region to create must be page aligned");
    ASSERT(IS_PAGE_ALIGNED(size), "Size of IO memory region to create must be page aligned");
    ASSERT(regionName != NULL, "A name for the IO memory region must be supplied");
    
    size_t continousFreeSpace = 0;

    for(Address curAddr = iomemStart; curAddr < iomemStart + iomemSize; curAddr += PAGE_SIZE)
    {
        if(continousFreeSpace >= size)
        {
            IO_MEMORY_MANAGER_DEBUG_MSG("Seems we found a hole large enough to fit our request at " << hex << (curAddr - size));
            return Allocate(curAddr-size, physicalAddress, size, regionName);
        }

        if(FindRegionEnclosingVirtualAddress(curAddr) == NULL)
            continousFreeSpace += PAGE_SIZE;
        else
            continousFreeSpace = 0;
    }
    
    return NULL;
}

Address IOMemoryManager::TranslatePhysicalAddress(Address physicalAddress)
{
    IO_MEMORY_MANAGER_DEBUG_MSG("Trying to translate physical address " << hex << physicalAddress << " to appropriate virtual address in I/O Memory");
    
    IOMemoryRegion* iomemregion = VirtualMemoryManager::GetInstance()->IOMemory()->FindRegionEnclosingPhysicalAddress(physicalAddress);
    if(iomemregion == NULL)
    {
        IO_MEMORY_MANAGER_DEBUG_MSG("There is no known I/O Memory region which spans over the given physical address " << hex << physicalAddress);
        return NULL;
    }
    
    Address offsetInRegion = (physicalAddress & PAGEALIGN_MASK) - iomemregion->StartAddressPhysical();
    
    Address offset = iomemregion->StartAddress() - (physicalAddress & PAGEALIGN_MASK);
    IO_MEMORY_MANAGER_DEBUG_MSG("Offset between virtual and physical address is " << hex << offset);
    IO_MEMORY_MANAGER_DEBUG_MSG("Resulting virtual address is " << hex << physicalAddress + offset + offsetInRegion);
    
    return physicalAddress + offset + offsetInRegion;
}

IOMemoryRegion* IOMemoryManager::Allocate(Address virtualAddress, Address physicalAddress, size_t size, const char* regionName)
{
    IOMemoryRegion* region = new IOMemoryRegion(virtualAddress, physicalAddress, size, regionName);
    
    for(Address i = region->startAddress; i < region->startAddress + region->size; i += PAGE_SIZE)
    {
        ASSERT(!CurrentHAL->GetPhysicalMemoryAllocator()->IsFree(physicalAddress), "Physical frame to be mapped manually into I/O Memory is not marked as used");
        ASSERT(!CurrentHAL->GetPaging()->IsPresent(i), "Virtual address which should be mapped manually to a physical frame is already present");
        
        CurrentHAL->GetPaging()->MapAddress(
            i,
            physicalAddress,
            true,   /*Writable*/
            false   /*Usermode*/
        );
        //this->manager->PhysicalAllocator()->MarkAsUsed(physAddr);
        
        physicalAddress += PAGE_SIZE;
    }
    
    AddRegion(region);
    return region;
}

void IOMemoryManager::DumpRegions(BaseOutputDevice* c)
{
    for(IOMemoryRegion* curRegion = regionList.Head(); curRegion != NULL; curRegion = regionList.GetNext(curRegion))
    {
        *c << "IOMEMSPACE: " << "\tName: " << curRegion->name << endl;
        *c << "IOMEMSPACE: " << "\tVirtualStartAddress: " << hex << (unsigned int)curRegion->startAddress << endl;
        *c << "IOMEMSPACE: " << "\tPhysicalStartAddress: " << hex << (unsigned int)curRegion->startAddressPhysical << endl;
        *c << "IOMEMSPACE: " << "\tSize: " << hex << (unsigned int)curRegion->size << endl;
        *c << endl;
    }
}
