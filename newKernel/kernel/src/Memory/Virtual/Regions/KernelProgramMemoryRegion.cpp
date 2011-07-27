#include <kernel/Memory/Virtual/Regions/KernelProgramMemoryRegion.h>
#include <halinterface/HAL.h>

using namespace Memory;
using namespace Arch;

KernelProgramMemoryRegion::KernelProgramMemoryRegion(Address RegionStart, size_t RegionSize, const char* RegionName, AllocationFlags RegionFlags)
: VirtualMemoryRegion(RegionStart, RegionSize, RegionName, RegionFlags)
{
    //Sanity of RegionStart and RegionSize is checked in VirtualMemoryRegion
    
    Address curAddress = RegionStart;
    size_t length = RegionSize;
    
    while(length != 0)
    {
        //by getting the physical address every time, we slow down things a bit, but we ensure
        //that the regions don't need to be continously in physical memory.
        //It's done only at startup of the kernel, so it should be OK
        CurrentHAL->GetPhysicalMemoryAllocator()->MarkAsUsed(CurrentHAL->GetPaging()->GetPhysicalAddress(curAddress));
        
        curAddress += PAGE_SIZE;
        length -= PAGE_SIZE;
    }
}
