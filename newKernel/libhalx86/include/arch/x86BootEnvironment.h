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
    
        virtual size_t GetInstalledMemory() { return memory; }
        virtual const char* GetKernelCommandline() { return cmdLine; }
        
        virtual Address GetModuleLocation() { return moduleLocation; }
        virtual size_t GetModuleSize() { return moduleSize; }
    
        uint32_t GetBIOSMemregionCount() { return biosMemregionCount; }
        KernelInformationMemoryRegion* GetBIOSMemregion(uint32_t idx)
        {
            ASSERT(idx < biosMemregionCount, "Index out of bounds");
            return biosMemregions + idx;
        }
        
        uint32_t GetProgramRegionCount() { return programRegionCount; }
        KernelInformationProgramRegion* GetProgramRegion(uint32_t idx)
        {
            ASSERT(idx < programRegionCount, "Index out of bounds");
            return programRegions + idx;
        }
    
    private:
        char* cmdLine;
        size_t memory;
        
        Address moduleLocation;
        size_t moduleSize;
        
        uint32_t biosMemregionCount;
        KernelInformationMemoryRegion* biosMemregions;
        
        uint32_t programRegionCount;
        KernelInformationProgramRegion* programRegions;
        
        uint8_t* symtab;
        uint8_t* strtab;
        
        size_t symtabSize;
        size_t strtabSize;
    };
}

#endif
