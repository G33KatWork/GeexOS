#ifndef _HAL_INTERFACES_BOOTENVIRONMENT_H
#define _HAL_INTERFACES_BOOTENVIRONMENT_H

#include <types.h>

namespace Arch
{
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
        
        /// Returns the number of ELF sections
        virtual uint32_t GetELFNum() = 0;
        virtual uint32_t GetELFSize() = 0;
        virtual uint32_t GetELFAddress() = 0;
        virtual uint32_t GetELFshndx() = 0;
    };
}

#endif
