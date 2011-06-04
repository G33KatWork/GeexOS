#include <types.h>
#include <limits.h>

#include <halinterface/HAL.h>

#include <kernel/debug.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>

using namespace Arch;
using namespace Debug;
using namespace Memory;

int main()
{
    #ifdef SERIAL_DEBUG
        CurrentHAL->SetCurrentDebugOutputDeviceType(Debug::Serial);
    #else
        CurrentHAL->SetCurrentDebugOutputDeviceType(Debug::TextMonitor);
    #endif
    CurrentHAL->GetCurrentDebugOutputDevice()->Clear();
    
    MAIN_DEBUG_MSG("GeexOS Kernel booting...");
    
    CurrentHAL->Initialize();
    
    MAIN_DEBUG_MSG("Kernel commandline: " << CurrentHAL->GetBootEnvironment()->GetKernelCommandline());
    
    //Initialize Memory
    VirtualMemoryManager::GetInstance()->Init();
    MAIN_DEBUG_MSG("Virtual memory manager instace created...");
    
    //Build virtual memory space for kernel ELF
    VirtualMemoryManager::GetInstance()->KernelSpace(
        new VirtualMemorySpace(VirtualMemoryManager::GetInstance(), "KernelSpace", CurrentHAL->GetPaging()->GetKernelDirectory())
    );
    VirtualMemoryManager::GetInstance()->SetCurrentMemorySpace(VirtualMemoryManager::GetInstance()->KernelSpace()); //Just to be sure...
    MAIN_DEBUG_MSG("Virtual memory space for kernel created...");
    
    
    
    CurrentHAL->InitializationDone();
    
    while(1);
}
