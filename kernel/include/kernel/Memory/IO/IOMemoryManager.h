#ifndef IOMEMORYMANAGER_H_
#define IOMEMORYMANAGER_H_

#include <lib/types.h>
#include <kernel/Memory/IO/IOMemoryRegion.h>
#include <kernel/IO/CharacterOutputDevice.h>

namespace Memory
{
    class IOMemoryManager
    {
    public:
        IOMemoryManager(Address IOMemoryStart, size_t IOMemorySize);
        
        void AddRegion(IOMemoryRegion* region);
        void RemoveRegion(IOMemoryRegion* region);
        
        IOMemoryRegion* FindRegionByName(const char* regionName);
        IOMemoryRegion* FindRegionEnclosingPhysicalAddress(Address physicalAddress);
        IOMemoryRegion* FindRegionEnclosingVirtualAddress(Address virtualAddress);
    
        IOMemoryRegion* MapPhysical(Address physicalAddress, size_t size, const char* regionName);
        
        void DumpRegions(IO::CharacterOutputDevice& c);
        
    private:
        Address iomemStart;
        size_t iomemSize;
        
        IOMemoryRegion* RegionListHead;
        
        IOMemoryRegion* Allocate(Address virtualAddress, Address physicalAddress, size_t size, const char* regionName);
    };
}
#endif
