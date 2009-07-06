#include <kernel/Memory/Heap.h>
#include <kernel/Memory/Paging.h>
#include <kernel/global.h>

using namespace Memory;
using namespace IO;

Heap::Heap(Address location, unsigned int size, unsigned int initialSize)
{
    
}

void* Heap::Allocate(size_t len, bool pageAlign)
{
    return NULL;
}

void Heap::Deallocate(void* p)
{
    
}
