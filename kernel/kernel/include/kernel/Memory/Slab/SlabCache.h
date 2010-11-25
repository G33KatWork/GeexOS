#ifndef _SLAB_ALLOCATOR_SLABCACHE_H_
#define _SLAB_ALLOCATOR_SLABCACHE_H_

#include <arch/types.h>
#include <types.h>
#include <kernel/DataStructures/DoublyLinkedList.h>
#include <kernel/Memory/Slab/Slab.h>

namespace Memory
{
    namespace Slab
    {
        class SlabAllocator;
        
        class SlabCache : public DataStructures::DoublyLinkedListLinkImpl<SlabCache>
        {        
        protected:
            SlabCache(const char* Name, int Align, int Size, SlabAllocator* ParentAllocator);
            virtual ~SlabCache() {}
        
            DataStructures::DoublyLinkedList<Slab> fullSlabList;
            DataStructures::DoublyLinkedList<Slab> partialSlabList;
            DataStructures::DoublyLinkedList<Slab> freeSlabList;
        
            SlabAllocator* allocator;
            char name[SLAB_MAX_NAMELEN];
            size_t objSize;
            size_t order;
            size_t objectsPerSlab;
            //size_t objectsAllocated;
            //size_t objectsActive;
        
            static size_t EstimateNrObjects(size_t order, size_t objSize, size_t* nr, size_t* wastage);
            static size_t GetObjectCount(size_t* objSize, size_t align, size_t* order, size_t* left);
        
            Slab* GetNonEmptySlab();
        
            /* grow the cache by one slab */
            virtual Slab* Grow() = 0;
        
        public:
            /* allocate a new object in a Slab inside this SlabCache */
            virtual void* AllocateObject() = 0;
            virtual void FreeObject(void* object) = 0;
        
            inline size_t GetObjectSize() { return objSize; }
            inline size_t GetOrder() { return order; }
            inline const char* GetName() const { return name; }
        };
    }
}
#endif
