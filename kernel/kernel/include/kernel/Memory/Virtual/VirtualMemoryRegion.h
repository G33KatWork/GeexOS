#ifndef VIRTUALMEMORYREGION_H_
#define VIRTUALMEMORYREGION_H_

#include <types.h>
#include <kernel/global.h>
#include <arch/HAL.h>
#include <kernel/DataStructures/DoublyLinkedList.h>

namespace Memory
{
    /*struct _AllocationFlags
    {
        unsigned int usermode:1;
        unsigned int writable:1;
        unsigned int executable:1;
        
        unsigned int unused:29;
    };*/
    
    #define ALLOCFLAG_NONE          0x00
    #define ALLOCFLAG_USERMODE      0x01
    #define ALLOCFLAG_WRITABLE      0x02
    #define ALLOCFLAG_EXECUTABLE    0x04
    
    typedef uint32_t AllocationFlags;
    
    /*
     * This class defines a region inside a virtual memory space
     */
    class VirtualMemoryRegion : public DataStructures::DoublyLinkedListLinkImpl<VirtualMemoryRegion>
    {
    friend class VirtualMemorySpace;
        
    public:
        VirtualMemoryRegion(Address RegionStart, size_t RegionSize, const char* RegionName, AllocationFlags RegionFlags)
        {
            ASSERT(IS_PAGE_ALIGNED(RegionStart), "Start address needs to be page aligned.");
            ASSERT(RegionSize % PAGE_SIZE == 0, "Size of a region must be a multiple of the page size.");

            startAddress = RegionStart;
            size = RegionSize;
            name = RegionName;
            flags = RegionFlags;
        }

		virtual ~VirtualMemoryRegion()
		{
			UnmapPages(this->startAddress, this->size);
		}
        
        Address StartAddress() { return startAddress; }
        size_t Size() { return size; }
        const char* Name() { return name; }
		
		//FIXME: What were these for? ;)
		/*virtual void* AllocateMemory(size_t size) = 0;
		virtual void DeallocateMemory(void* beginning) = 0;*/
		
        virtual bool HandlePageFault(Address faultingAddress)
        {
            PANIC("PageFault occured at " << Debug::hex << faultingAddress << " in VirtualMemoryRegion " << name << " and it couldn't be resolved by any class lying over this. Bad for you!");
            return false;
        }

    protected:
        void MapFreshPages(Address start, size_t length);
        void UnmapPages(Address start, size_t length);

	protected:
        AllocationFlags flags;
        Address startAddress;
        size_t size;
        const char* name;
    };
}
#endif
