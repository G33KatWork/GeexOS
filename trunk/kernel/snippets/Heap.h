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
		struct __freelist {
			size_t sz;
			struct __freelist *nx;
		};
		
		struct __freelist *__flp;   /* memory block list */
        Address __brkval;           /* Beginning of last free space */
        Address endAssigned;        /* First address where no frame is mapped yet */
		Address begin;              /* Beginning of heap */
		unsigned int maxSize;       /* max size of heap in kB */
		
        void Expand();
    };
}
#endif
