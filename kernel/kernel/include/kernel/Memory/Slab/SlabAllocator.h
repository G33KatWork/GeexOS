#ifndef _SLAB_ALLOCATOR_H_
#define _SLAB_ALLOCATOR_H_

#include <arch/types.h>
#include <types.h>
#include <kernel/Memory/Virtual/Regions/BuddyAllocatedMemoryRegion.h>
#include <kernel/Memory/Slab/SlabCache.h>
#include <kernel/DataStructures/DoublyLinkedList.h>

namespace Memory
{
    namespace Slab
    {
        class SlabAllocator : public BuddyAllocatedMemoryRegion
        {
        private:
            SlabCache* cacheHead;
            DataStructures::DoublyLinkedList<SlabCache> cacheList;
	
        public:
            SlabAllocator(Address RegionStart, size_t RegionSize);
            ~SlabAllocator() {}
        
            SlabCache* CreateCache(const char* cacheName, size_t objectSize, size_t alignment);
            void DestroyCache(SlabCache* cache);
            
            void FreeUnusedMemory();
        };
        
        void* AllocateFromSizeSlabs(size_t size);
        void FreeFromSizeSlabs(void* address);
        void InitializeSizeCaches(SlabAllocator* allocator);
    }
}

size_t static inline GetOrder(size_t size)
{
    size_t order = -1;
    size = (size - 1) >> (PAGE_SHIFT - 1);
    do { 
        size >>= 1;
        order++;
    } while(size);
    return order;
}

#endif
