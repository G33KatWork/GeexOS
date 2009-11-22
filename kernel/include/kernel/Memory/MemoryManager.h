#ifndef MEMORY_MANAGER_H_
#define MEMORY_MANAGER_H_

#include <lib/types.h>
#include <kernel/Memory/IMemoryAllocator.h>
#include <kernel/Memory/IPhysicalMemoryManager.h>
#include <kernel/Memory/Stack.h>

namespace Memory
{
    class MemoryManager
    {
    public:
        IMemoryAllocator* GetAllocator()
        {
            return allocator;
        }
        
        void SetAllocator(IMemoryAllocator *alloc)
        {
            allocator = alloc;
        }
        
        void SetPhysicalMemoryManager(IPhysicalMemoryManager *phys)
        {
            physical = phys;
        }
        
        Stack* GetKernelStack()
        {
            return kernelStack;
        }
        
        void SetKernelStack(Stack* s)
        {
            kernelStack = s;
        }
        
        void* kmalloc(size_t s, bool pageAlign)
        {
            return allocator->Allocate(s, pageAlign);
        }
        
        void kfree(void* p)
        {
            allocator->Deallocate(p);
        }
        
        Address AllocateFrame()
        {
            return physical->AllocateFrame();
        }
        
        void DeallocateFrame(Address a)
        {
            physical->DeallocateFrame(a);
        }
        
    private:
        IMemoryAllocator *allocator;
        IPhysicalMemoryManager *physical;
        
        Stack* kernelStack;
    };
}
#endif
