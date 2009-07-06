#ifndef PAGING_H_
#define PAGING_H_

#include <lib/types.h>
#include <arch/paging.h>

using namespace Arch;

namespace Memory
{
    class Paging
    {
    public:
        static Paging* GetInstance();
        
        void Init(void);
        
        Address GetPhysicalAddress(Address virtualaddr);
        void MapAddress(Address virt, Address phys, bool readwrite, bool usermode);
        
        void SwitchCurrentPageDirectory(PageDirectory* dir);
        
        PageDirectory* GetKernelDirectory() { return kernel_directory; }
        
    private:
        static Paging* instance;
        
        Paging(){};
        
        PageDirectory *kernel_directory;
        PageDirectory *current_directory;
        
        PageTable *lowpagetable;
    };
}
#endif
