#ifndef HEAP_H_
#define HEAP_H_

#include <lib/types.h>
#include <kernel/Memory/IMemoryAllocator.h>

namespace Memory
{
    class Heap : public IMemoryAllocator
    {
	public:
        Heap(Address location, unsigned int size, unsigned int initialSize);
        
        void* Allocate(size_t len, bool pageAlign);
        void Deallocate(void* p);
        
    private:
		
    };
}
#endif
