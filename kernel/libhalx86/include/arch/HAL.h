#ifndef _ARCHX86_HAL_H
#define _ARCHX86_HAL_H

#include <halinterface/HAL.h>
#include <arch/TextmodeDebugOutput.h>
#include <arch/SerialDebugOutput.h>
#include <arch/x86Paging.h>
#include <arch/BitfieldPhysicalMemoryAllocator.h>
#include <arch/AddressLayout.h>

#include <arch/internal/x86ResolveableException.h>

namespace Arch
{
    class x86HAL : public HAL
    {
    public:
        x86HAL();
        virtual ~x86HAL() {}
        
        virtual void Initialize();
        virtual void InitializationDone();
        virtual bool IsHALFullyInitialized() { return halFullyInitialized; }
        
        virtual void EnableInterruptsOnCurrentCPU();
        virtual void DisableInterruptsOnCurrentCPU();
        
        virtual void GetCPUVendorName(char* buf);
        
        virtual void HaltCurrentCPU();
        
        virtual BasePaging* GetPaging();
        virtual BootEnvironment* GetBootEnvironment() { return bootenv; }
        virtual Debug::BaseOutputDevice* GetCurrentDebugOutputDevice();
        virtual void SetCurrentDebugOutputDeviceType(Debug::DebugOutputDeviceType type);
        
        virtual BasePhysicalMemoryAllocator* GetPhysicalMemoryAllocator();
        virtual BaseInterruptDispatcher* GetInterruptDispatcher();
        
    private:
        void reserveBIOSMemregions();
        void initializeBootGraphics();
        
    private:
        bool halFullyInitialized;

        BasePaging *paging;
        BootEnvironment* bootenv;
        Debug::NullDebugOutputDevice nullDebug;
        Debug::BaseOutputDevice* serialDebug;
        Debug::DebugOutputDeviceType currentDebugDevice;
        
        BitfieldPhysicalMemoryAllocator* physicalAllocator;
        BaseInterruptDispatcher* interruptDispatcher;
        
        Arch::x86ResolveableException* pageFaultHandler;
    };
}

#endif
