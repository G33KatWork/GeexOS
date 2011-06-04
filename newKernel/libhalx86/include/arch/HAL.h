#ifndef _ARCHX86_HAL_H
#define _ARCHX86_HAL_H

#include <halinterface/HAL.h>
#include <arch/TextmodeDebugOutput.h>
#include <arch/SerialDebugOutput.h>
#include <arch/x86Paging.h>
#include <arch/BitfieldPhysicalMemoryAllocator.h>
#include <arch/AddressLayout.h>

namespace Arch
{
    class x86HAL : public HAL
    {
    public:
        x86HAL();
        virtual ~x86HAL() {}
        
        virtual void Initialize();
        virtual void InitializationDone();
        
        virtual void EnableInterruptsOnCurrentCPU();
        virtual void DisableInterruptsOnCurrentCPU();
        
        virtual void GetCPUVendorName(char* buf);
        
        virtual void HaltCurrentCPU();
        
        virtual BasePaging* GetPaging();
        virtual BootEnvironment* GetBootEnvironment() { return bootenv; }
        virtual Debug::BaseDebugOutputDevice* GetCurrentDebugOutputDevice();
        virtual void SetCurrentDebugOutputDeviceType(Debug::DebugOutputDeviceType type);
        
        virtual BasePhysicalMemoryAllocator* GetPhysicalMemoryAllocator();
        
    private:
        void reserveBIOSMemregions();
        
    private:
        BasePaging *paging;
        BootEnvironment* bootenv;
        Debug::NullDebugOutputDevice nullDebug;
        Debug::BaseDebugOutputDevice* textDebug;
        Debug::BaseDebugOutputDevice* serialDebug;
        Debug::DebugOutputDeviceType currentDebugDevice;
        
        BitfieldPhysicalMemoryAllocator* physicalAllocator;
    };
}

#endif
