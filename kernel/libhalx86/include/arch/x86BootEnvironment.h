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

        //HAL-internal functions
        virtual PMEMORY_DESCRIPTOR GetMemoryRegions(size_t* totalCount);
        virtual PMEMORY_DESCRIPTOR GetMemoryRegionByType(PMEMORY_DESCRIPTOR start, MemoryType type);
    
    private:
        KernelInformation* kernelInformation;

        uint64_t memory;
    };
}

#endif
