#ifndef IOMEMORYMANAGER_H_
#define IOMEMORYMANAGER_H_

#include <types.h>
#include <kernel/Memory/IO/IOMemoryRegion.h>
#include <halinterface/BaseOutputDevice.h>
#include <kernel/DataStructures/DoublyLinkedList.h>

namespace Memory
{
    class IOMemoryManager
    {
    public:
        IOMemoryManager(Address IOMemoryStart, size_t IOMemorySize);
        
        void AddRegion(IOMemoryRegion* region);
        //void RemoveRegion(IOMemoryRegion* region);
        
        IOMemoryRegion* FindRegionByName(const char* regionName);
        IOMemoryRegion* FindRegionEnclosingPhysicalAddress(Address physicalAddress);
        IOMemoryRegion* FindRegionEnclosingVirtualAddress(Address virtualAddress);
    
        IOMemoryRegion* MapPhysical(Address physicalAddress, size_t size, const char* regionName);
        
        Address TranslatePhysicalAddress(Address physicalAddress);
        
        void DumpRegions(Debug::BaseOutputDevice* c);
        
    private:
        Address iomemStart;
        size_t iomemSize;
        
        DataStructures::DoublyLinkedList<IOMemoryRegion> regionList;
        
        IOMemoryRegion* Allocate(Address virtualAddress, Address physicalAddress, size_t size, const char* regionName);
    };
}
#endif
