#ifndef _HAL_INTERFACES_HAL_H
#define _HAL_INTERFACES_HAL_H

#include <types.h>
#include <halinterface/ClockSource.h>
#include <halinterface/BaseInterruptDispatcher.h>
#include <halinterface/BasePaging.h>
#include <halinterface/BootEnvironment.h>

namespace Arch
{
    class HAL
    {
    public:
        virtual void Initialize() = 0;
        virtual void InitializationDone() = 0;
        
        virtual void SetupArchMemRegions() = 0;
        
        virtual void EnableInterrupts() = 0;
        virtual void DisableInterrupts() = 0;
        
        virtual void GetCPUVendor(char* buf) = 0;
    
        virtual void HaltMachine() = 0;
        
        virtual BaseInterruptDispatcher* GetInterruptDispatcher() = 0;
        virtual BasePaging* GetPaging() = 0;
        
        virtual ClockSource* GetHardwareClockSource() = 0;
        
        virtual BootEnvironment* GetBootEnvironment() = 0;
        
        virtual Address GetStackPointer() = 0;
        virtual void SetStackPointer(Address NewPointer) = 0;
        virtual Address GetFramePointer() = 0;
        virtual void SetFramePointer(Address NewPointer) = 0;
    };
    
    //Define instance of HAL for easy access from anywhere
    //The instance itself is a static object created in x86HAL.cpp
    extern class HAL* CurrentHAL;
}

#endif
