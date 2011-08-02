#ifndef _HAL_INTERFACES_HAL_H
#define _HAL_INTERFACES_HAL_H

#include <halinterface/BootEnvironment.h>
#include <halinterface/BaseOutputDevice.h>
#include <halinterface/BasePaging.h>
#include <halinterface/BasePhysicalMemoryAllocator.h>
#include <halinterface/BaseInterruptDispatcher.h>

namespace Arch
{
    /**
     * The HAL encapsulates all platform specific information and actions the
     * kernel needs in its early initialization process when no modules are loaded
     * yet. The specific platform implementation is linked statically into the
     * kernel and available directly after coming out of the bootloader.
    **/
    class HAL
    {
    public:
        /**
         * This method is responsible for early initialization of the platform.
         * This includes setting up Paging.
        **/
        virtual void Initialize() = 0;
        
        /**
         * This method is called to finalize the early initialization.
         * The HAL assumes that the VirtualMemoryManager is initialized here.
        **/
        virtual void InitializationDone() = 0;
        
        /**
         * This call globally enables interrupt handling by setting the
         * appropriate flag inside the current CPU. Saved task states won't
         * be modified. As soon as a task switch will occur, interrupts
         * are enabled again.
        **/
        virtual void EnableInterruptsOnCurrentCPU() = 0;
        
        /**
         * Globally disables interrupts by deleting the CPU flag.
         * The same restrictions apply as in EnableInterrupts()
         *
         * @todo
         * return a bool which indicates if interrupts were enabled.
         * If not, reenabling should not take place.
        **/
        virtual void DisableInterruptsOnCurrentCPU() = 0;
        
        /**
         * This method puts a string containing the CPU vendors name
         * into a given buffer. The caller is responsible for providing
         * a buffer with proper size.
        **/
        virtual void GetCPUVendorName(char* buf) = 0;
        
        /**
         * This method should halt all CPUs and disable all interrupts.
         * Mainly this will be called in the event of a kernel panic
         * or a shutdown/reboot.
        **/
        virtual void HaltCurrentCPU() = 0;
        
        /**
         * Returns an instance of the Paging subsystem for the platform
        **/
        virtual BasePaging* GetPaging() = 0;
        
        /**
         * Returns a BootEnvironment containing information like the ELF headers
         * of a kernel or the kernel command line
        **/
        virtual BootEnvironment* GetBootEnvironment() = 0;
        
        /**
         * Returns an instance to the currently used debug output device
        **/
        virtual Debug::BaseOutputDevice* GetCurrentDebugOutputDevice() = 0;
        
        /**
         * Switches the currently used debug output device to the given type
        **/
        virtual void SetCurrentDebugOutputDeviceType(Debug::DebugOutputDeviceType type) = 0;
        
        /**
         * Returns the physical memory allocator for this architecture
        **/
        virtual BasePhysicalMemoryAllocator* GetPhysicalMemoryAllocator() = 0;
        
        /**
         * Returns the interrupt dispatcher for this architecture
        **/
        virtual BaseInterruptDispatcher* GetInterruptDispatcher() = 0;
    };
    
    /**
     * This definition of a pointer holds the instance of the current HAL.
     * It can be accessed everywhere from the kernel by including HAL.h
     * It is instanciated inside the platform specific implementation of the HAL.
    **/
    extern class HAL* CurrentHAL;
}

#endif
