#ifndef _ARCHX86_BOOTENVIRONMENT_H
#define _ARCHX86_BOOTENVIRONMENT_H

#include <types.h>
#include <halinterface/BootEnvironment.h>
#include <arch/internal/KernelInfo.h>
#include <kernel/global.h>

namespace Arch
{
    class x86BootEnvironment : public BootEnvironment
    {
    public:
        x86BootEnvironment(KernelInformation* i);

        //FIXME: make this actually do something...
        virtual uint64_t GetInstalledMemory() { return memory; }
        virtual const char* GetKernelCommandline() { return ""; }
        
        virtual KernelBootModuleRepository* GetBootModuleRepository() { return NULL; }
        virtual size_t GetBootModuleRepositorySize() { return 0; }
        
        virtual size_t GetProgramRegionCount() { return 0; }
        virtual KernelProgramRegion* GetProgramRegion(size_t idx)
        {
            return NULL;
        }

        //HAL-internal functions
        virtual PMEMORY_DESCRIPTOR GetMemoryRegions(size_t* totalCount);
        virtual PMEMORY_DESCRIPTOR GetMemoryFirstRegionByType(MemoryType type);
    
    private:
        KernelInformation* kernelInformation;

        uint64_t memory;
    };
}

#endif
