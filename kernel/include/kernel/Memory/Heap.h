#ifndef HEAP_H_
#define HEAP_H_

#include <lib/types.h>
#include <kernel/Memory/IMemoryAllocator.h>
#include <kernel/DataStructures/OrderedArray.h>

using namespace DataStructures;

namespace Memory
{
    typedef struct header_t;
    
    struct node
    {
        size_t size;
        bool free;
        struct node* next;
    };
    
    typedef struct node list_item;
    
    class Heap : public IMemoryAllocator
    {
    private:
        Address startAddress;
        Address endAddress;
        Address maxAddress;
        
        void expand(size_t newSize);
        void expandTo(Address upperAddress);
        list_item* findFirstFit(size_t size, bool pageAlign);
            
	public:
        Heap(Address location, size_t size, size_t initialSize);
        
        void* Allocate(size_t len, bool pageAlign);
        void Deallocate(void* p);
        
        size_t getCurrentSize()
        {
            return this->endAddress - this->startAddress;
        }
        
    };
}
#endif
