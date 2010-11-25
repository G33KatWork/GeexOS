#ifndef _SLAB_ALLOCATOR_SLAB_H_
#define _SLAB_ALLOCATOR_SLAB_H_

#include <arch/types.h>
#include <types.h>
#include <kernel/DataStructures/DoublyLinkedList.h>

#define SLAB_MAX_NAMELEN    40
#define SLAB_HWCACHE_ALIGN  0x20    //FIXME: make processor independent
#define SLAB_MAXORDER       5
#define SLAB_LIMIT          (PAGE_SIZE >> 3)    //if objects are bigger than 1/8th of a page size, they are off-slab

//End marker of free-list
#define SLAB_BUF_END        (((uint32_t)(~0U))-0)

namespace Memory
{
    namespace Slab
    {
        class SlabCache;
    
        struct Slab : public DataStructures::DoublyLinkedListLinkImpl<Slab>
        {
            SlabCache* cache;
            Address objectStart;
            uint32_t freeIndex;     //Index to next free object in the free-array
            uint32_t inUse;
        
            bool IsFull() { return this->freeIndex == SLAB_BUF_END; }
            bool IsEmpty() { return inUse == 0; }
        };
    }
}
#endif
