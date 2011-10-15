#ifndef IOMEMORYREGION_H_
#define IOMEMORYREGION_H_

#include <types.h>
#include <kernel/DataStructures/DoublyLinkedList.h>

namespace Memory
{
    class IOMemoryRegion : public DataStructures::DoublyLinkedListLinkImpl<IOMemoryRegion>
    {
    friend class IOMemoryManager;
        
    public:
        IOMemoryRegion(Address RegionStart, Address PhysicalStart, size_t RegionSize, const char* RegionName)
        {
            startAddress = RegionStart;
            startAddressPhysical = PhysicalStart;
            size = RegionSize;
            name = RegionName;
        }
        
        Address StartAddress() { return startAddress; }
        Address StartAddressPhysical() { return startAddressPhysical; }
        size_t Size() { return size; }
        const char* Name() { return name; }
        
    private:
        Address startAddress;
        Address startAddressPhysical;
        size_t size;
        const char* name;
    };
}
#endif
