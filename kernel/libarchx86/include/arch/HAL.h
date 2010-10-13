#ifndef _ARCHX86_HAL_H
#define _ARCHX86_HAL_H

#include <halinterface/HAL.h>
#include <arch/internal/PIT.h>
#include <arch/x86ThreadContext.h>
#include <arch/internal/SchedulingHelper.h>
#include <arch/x86BootEnvironment.h>
#include <arch/TextmodeDebugOutput.h>
#include <arch/SerialDebugOutput.h>

namespace Arch
{
    class x86HAL : public HAL
    {
    public:
        x86HAL();
        virtual ~x86HAL() {}
        
        virtual void Initialize();
        virtual void InitializationDone();
        
        virtual void SetupArchMemRegions();
        
        virtual void EnableInterrupts();
        virtual void DisableInterrupts();
        
        virtual void GetCPUVendor(char* buf);
    
        virtual void HaltMachine();
        
        virtual BaseInterruptDispatcher* GetInterruptDispatcher();
        virtual BasePaging* GetPaging();
        virtual ClockSource* GetHardwareClockSource();
        virtual BootEnvironment* GetBootEnvironment() { return bootenv; }
        virtual Debug::BaseDebugOutputDevice* GetCurrentDebugOutputDevice();
        virtual void SetCurrentDebugOutputDeviceType(Debug::DebugOutputDeviceType type);
        
        virtual Address GetStackPointer() { return readStackPointer(); }
        virtual void SetStackPointer(Address NewPointer) { writeStackPointer(NewPointer); }
        virtual Address GetFramePointer() { return readBasePointer(); }
        virtual void SetFramePointer(Address NewPointer) { writeBasePointer(NewPointer); }
    
    private:
        BaseInterruptDispatcher* ird;
        BasePaging *paging;
        BootEnvironment* bootenv;
        Debug::NullDebugOutputDevice nullDebug;
        Debug::BaseDebugOutputDevice* textDebug;
        Debug::BaseDebugOutputDevice* graphicalDebug;
        Debug::BaseDebugOutputDevice* serialDebug;
        Debug::DebugOutputDeviceType currentDebugDevice;
        
        ClockSource clk;
        PIT* pit;
    };
    
    //Export the ThreadContext type
    typedef x86ThreadContext ThreadContext;
}

#endif