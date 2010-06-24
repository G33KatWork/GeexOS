#ifndef KERNELSTACKMEMORYREGION_H_
#define KERNELSTACKMEMORYREGION_H_

#include <lib/types.h>
#include <kernel/global.h>
#include <kernel/Memory/Virtual/Regions/LazyMemoryRegion.h>

namespace Memory
{
    /*  This is the main kernel stack class
     *  It's mainly used during bootup to move the stack around
     *  All kernel thread stacks are managed in the KernelThreadStackRegion
     */
    class KernelStackMemoryRegion : public LazyMemoryRegion
    {
        friend class VirtualMemorySpace;
    private:
        size_t initialSize;
        size_t curSize;
    
    public:
        KernelStackMemoryRegion(Address RegionStart, size_t MaxRegionSize, size_t InitialSize, const char* RegionName);
        
        virtual void* AllocateMemory(size_t UNUSED(size)) { return NULL; }
        virtual void DeallocateMemory(void* UNUSED(beginning)) {}
        
        
        //Remember: Stack grows down!
        Address GetStartAddress() { return startAddress + size; }
        unsigned int GetSize() { return size; }
        Address GetEndAddress() { return startAddress; }
        
        void MoveCurrentStackHere(Address oldESP);
        void PrintStacktrace(unsigned int n = 15);
    };
}
#endif
