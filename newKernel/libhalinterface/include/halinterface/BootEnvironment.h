#ifndef _HAL_INTERFACES_BOOTENVIRONMENT_H
#define _HAL_INTERFACES_BOOTENVIRONMENT_H

#include <types.h>

namespace Arch
{
    class KernelProgramRegion
    {
    public:
        /// Virtual start address of mapped program section
        Address VirtualStart;
        
        /// Page-aligned length of mapped program section
        uint64_t Length;
        
        /// Readable flag
        bool Readable;
        
        /// Writable flag
        bool Writable;
        
        /// Executable flag
        bool Executable;
    };
    
    class BootEnvironment
    {
    protected:
        BootEnvironment() {}
        virtual ~BootEnvironment() {}
    
    public:
        /// Returns the amount of installed memory of the system
        virtual size_t GetInstalledMemory() = 0;
        
        /// Returns the kernel command line passed by the bootloader
        virtual const char* GetKernelCommandline() = 0;
        
        /// Returns the start address of the module BLOB in RAM
        virtual Address GetModuleLocation() = 0;
        
        /// Returns the length of the module BLOB
        virtual size_t GetModuleSize() = 0;
        
        /// Returns the amount of mapped program sections
        virtual size_t GetProgramRegionCount() = 0;
        
        /// Returns the i-th program section
        virtual KernelProgramRegion* GetProgramRegion(size_t idx) = 0;
    };
}

#endif
