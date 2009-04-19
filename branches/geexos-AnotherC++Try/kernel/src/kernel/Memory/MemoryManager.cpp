#include <kernel/Memory/MemoryManager.h>

using namespace Memory;

extern unsigned int end;

MemoryManager::MemoryManager()
{
    //FIXME: Why +0x10000? if not, it would overwrite the ELF tables used by stacktraces
    placement_address = (unsigned int)&end + 0x20000;
    heapInitialized = false;
}

bool MemoryManager::HeapInitialized()
{
    return heapInitialized;
}

void* MemoryManager::kmalloc(size_t size, bool pageAlign, Address* physicalAddress)
{
    unsigned int tmp;
    
    if(pageAlign && (placement_address & 0xFFFFF000))
    {
        // Align the placement address;
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    
    tmp = placement_address;
    
    if(physicalAddress != NULL)
        *physicalAddress = tmp;

    placement_address += size;
    return (void*)tmp;
}

void* MemoryManager::kcalloc(unsigned int n, size_t s, bool pageAlign, Address* physicalAddress)
{
    void* first = kmalloc(s, pageAlign, physicalAddress);
    for(unsigned int i = 1; i < n; i++)
        kmalloc(s, pageAlign);
    
    return first;
}

void MemoryManager::free(void* p)
{
    
}
