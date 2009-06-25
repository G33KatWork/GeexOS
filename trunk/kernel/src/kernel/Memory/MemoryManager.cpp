#include <kernel/Memory/MemoryManager.h>
#include <kernel/global.h>

using namespace Memory;

MemoryManager::MemoryManager()
{
    
}

IMemoryAllocator* MemoryManager::GetAllocator()
{
    return allocator;
}

void MemoryManager::SetAllocator(IMemoryAllocator *alloc)
{
    allocator = alloc;
}

void MemoryManager::SetPhysicalMemoryManager(IPhysicalMemoryManager *phys)
{
    physical = phys;
}

void* MemoryManager::kmalloc(size_t s)
{
    return allocator->Allocate(s, false);
}

void MemoryManager::kfree(void* p)
{
    allocator->Deallocate(p);
}

Address MemoryManager::AllocateFrame()
{
    return physical->AllocateFrame();
}

void MemoryManager::DeallocateFrame(Address a)
{
    physical->DeallocateFrame(a);
}
