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
        Paging(){};
        void Init(void);
        
    private:
        PageDirectory *kernel_directory;
        PageTable *lowpagetable;
    };
}
#endif
