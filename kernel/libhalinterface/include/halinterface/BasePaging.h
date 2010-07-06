#ifndef _HAL_INTERFACES_IPAGING_H
#define _HAL_INTERFACES_IPAGING_H

#include <types.h>

#define PAGE_ALIGN(x)       ((x & PAGEALIGN_MASK) + PAGE_SIZE)
#define IS_PAGE_ALIGNED(x)  ((x % PAGE_SIZE) == 0)

//The following macros need to be defined by the architecture implementation:
//PAGEALIGN_MASK            A mask to page align a value when and'ed
//OFFSET_MASK               A mask to get the offset in one page/frame when and'ed
//PAGE_SIZE                 The size of one page/frame in bytes

namespace Arch
{
    class BasePageDirectory;
    
    class BasePaging
    {
    public:
        BasePaging(){}
        virtual ~BasePaging() {}
        
        virtual void Init(void) = 0;
        virtual void InitDone(void) = 0;
        
        virtual Address GetPhysicalAddress(Address virtualaddr) = 0;
        virtual void MapAddress(Address virt, Address phys, bool readwrite, bool usermode) = 0;
        virtual void UnmapAddress(Address virt) = 0;
        virtual bool IsPresent(Address virt) = 0;
        
        virtual void SwitchCurrentPageDirectory(BasePageDirectory* dir) = 0;
        
        virtual BasePageDirectory* DeriveUserspaceDirectory() = 0;
        
        virtual BasePageDirectory* GetKernelDirectory() = 0;
    };

    class BasePageDirectory
    {
    };
}

#endif
