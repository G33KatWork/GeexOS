#ifndef MEMORY_MANAGER_H_
#define MEMORY_MANAGER_H_

#include <lib/types.h>
#include <kernel/Memory/IMemoryAllocator.h>
#include <kernel/Memory/IPhysicalMemoryManager.h>

namespace Memory
{
    class MemoryManager
    {
    public:
        MemoryManager();
        
        IMemoryAllocator* GetAllocator();
        void SetAllocator(IMemoryAllocator *alloc);
        
        void SetPhysicalMemoryManager(IPhysicalMemoryManager *phys);
        
        void* kmalloc(size_t s, bool pageAlign);
        void kfree(void* p);
        Address AllocateFrame();
        void DeallocateFrame(Address a);
        
    private:
        IMemoryAllocator *allocator;
        IPhysicalMemoryManager *physical;
    };
}
#endif
