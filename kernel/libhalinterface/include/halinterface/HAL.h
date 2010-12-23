#ifndef _HAL_INTERFACES_HAL_H
#define _HAL_INTERFACES_HAL_H

#include <halinterface/BaseTimer.h>
#include <halinterface/BaseInterruptDispatcher.h>
#include <halinterface/BasePaging.h>
#include <halinterface/BootEnvironment.h>
#include <halinterface/BaseDebugOutputDevice.h>
#include <halinterface/BaseProcessor.h>

namespace Arch
{
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
        **/
        virtual void InitializationDone() = 0;
        
        /**
         * This method is called after all memory related tasks are completed by
         * the kernel. Here the full memory manager as well as dynamic kernel
         * memory via the SLABAllocator is available.
        **/
        virtual void InitializationAfterMemoryAvailable() = 0;
        
        /**
         * The main kernel thread calls this method as soon as the
         * virtual memory manager is initialized. The HAL should register
         * platform specific memory regions
         * (IO Memory, memory holes, etc.) in this function.
         * Dynamic memory as the SLAB Allocator is not yet available here.
        **/
        virtual void SetupArchMemRegions() = 0;
        
        /**
         * This call globally enables interrupt handling by setting the
         * appropriate flag inside the current CPU. Saved task states won't
         * be modified. As soon as a task switch will occur, interrupts
         * are enabled again.
        **/
        virtual void EnableInterrupts() = 0;
        
        /**
         * Globally disables interrupts by deleting the CPU flag.
         * The same restrictions apply as in EnableInterrupts()
         *
         * @todo
         * return a bool which indicates if interrupts were enabled.
         * If not, reenabling should not take place.
        **/
        virtual void DisableInterrupts() = 0;
        
        /**
         * This method puts a string containing the CPU vendors name
         * into a given buffer. The caller is responsible for providing
         * a buffer with proper size.
        **/
        virtual void GetCPUVendor(char* buf) = 0;
        
        /**
         * This method returns an instance to the Processor the code is
         * currently running on.
        **/
        virtual BaseProcessor* GetCurrentProcessor() = 0;
        
        /**
         * This method should halt all CPUs and disable all interrupts.
         * Mainly this will be called in the event of a kernel panic
         * or a shutdown/reboot.
        **/
        virtual void HaltMachine() = 0;
        
        /**
         * Returns an instance of the platform specific interrupt handler
        **/
        virtual BaseInterruptDispatcher* GetInterruptDispatcher() = 0;
        
        /**
         * Returns an instance of the Paging subsystem for the platform
        **/
        virtual BasePaging* GetPaging() = 0;
        
        /**
         * Returns the currently used timer on the platform
        **/
        virtual BaseTimer* GetHardwareClockSource() = 0;
        
        /**
         * Returns a BootEnvironment containing information like the ELF headers
         * of a kernel or the kernel command line
        **/
        virtual BootEnvironment* GetBootEnvironment() = 0;
        
        /**
         * Returns an instance to the currently used debug output device
        **/
        virtual Debug::BaseDebugOutputDevice* GetCurrentDebugOutputDevice() = 0;
        
        /**
         * Switches the currently used debug output device to the given type
        **/
        virtual void SetCurrentDebugOutputDeviceType(Debug::DebugOutputDeviceType type) = 0;
        
        /**
         * Returns the current stackpointer after the function call
        **/
        virtual Address GetStackPointer() = 0;
        
        /**
         * Sets the stackpointer register to a new value. It is absolutely necessary that
         * this value is correct, otherwise a failure WILL occur when returning from this function
        **/
        virtual void SetStackPointer(Address NewPointer) = 0;
        
        /**
         * Returns the current frame pointer
        **/
        virtual Address GetFramePointer() = 0;
        
        /**
         * Sets the current framepointer register
        **/
        virtual void SetFramePointer(Address NewPointer) = 0;
    };
    
    /**
     * This definition of a pointer holds the instance of the current HAL.
     * It can be accessed everywhere from the kernel by including HAL.h
     * It is instanciated inside the platform specific implementation of the HAL.
    **/
    extern class HAL* CurrentHAL;
}

#endif
