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
        virtual size_t GetInstalledMemory() = 0;
        virtual const char* GetKernelCommandline() = 0;
        virtual uint32_t GetELFNum() = 0;
        virtual uint32_t GetELFSize() = 0;
        virtual uint32_t GetELFAddress() = 0;
        virtual uint32_t GetELFshndx() = 0;
    };
}

#endif
