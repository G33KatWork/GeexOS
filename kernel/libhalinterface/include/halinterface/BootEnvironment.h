#ifndef _HAL_INTERFACES_BOOTENVIRONMENT_H
#define _HAL_INTERFACES_BOOTENVIRONMENT_H

#include <types.h>
#include <halinterface/BaseOutputDevice.h>

#define BOOTMODULES_MAGIC   0xAABB0011

namespace Arch
{
    typedef struct 
    {
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
    } KernelProgramRegion;
    
    typedef struct
    {
        /// The magic value (0xAABB0011) of the repository for identification
        uint32_t Magic;
        
        /// The number of modules in the repository
        uint32_t ModuleCount;
        
        /// The offsets from the beginning of the repository to the beginning of the module
        uint32_t Offsets[1];
    } KernelBootModuleRepository;
    
    /**
     * The boot environment passed by the loader which loads the kernel and the boot module repository
     * It is responsible for copying every data into the kernel memory space since all passed addresses
     * are invalid after the kernel builds its own pagetables
    **/
    class BootEnvironment
    {
    protected:
        BootEnvironment() {}
        virtual ~BootEnvironment() {}
    
    public:
        /// Returns the amount of installed memory of the system
        virtual uint64_t GetInstalledMemory() = 0;
        
        /// Returns the kernel command line passed by the bootloader
        virtual const char* GetKernelCommandline() = 0;

        /// Dumps the boot environtment to the passed output device for debugging purposes
        virtual void DumpBootEnvironment(Debug::BaseOutputDevice* c) = 0;
    };
}

#endif
