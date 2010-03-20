#ifndef VIRTUALMEMORYREGION_H_
#define VIRTUALMEMORYREGION_H_

#include <lib/types.h>
#include <arch/Paging.h>
#include <kernel/global.h>

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
    class VirtualMemoryRegion
    {
    friend class VirtualMemorySpace;
        
    public:
        VirtualMemoryRegion(Address RegionStart, size_t RegionSize, const char* RegionName, AllocationFlags f);
        
        Address StartAddress;
        size_t Size;
        const char* Name;
        
        bool SwapOutRegion(Address address, size_t size);
        bool SwapInRegion(Address address, size_t size);
        
        AllocationFlags GetFlags() { return flags; }
        void SetFlags(AllocationFlags f);
    
    private:
        VirtualMemoryRegion *Next;
        AllocationFlags flags;
        
        //Set this variable to false is no swapping should ever occour
        //Standard is true
        bool SwappingAllowed;
    };
}
#endif
