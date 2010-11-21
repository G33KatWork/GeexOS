#ifndef _SLAB_ALLOCATOR_H_
#define _SLAB_ALLOCATOR_H_

#include <arch/types.h>
#include <types.h>
#include <arch/HAL.h>
#include <kernel/Memory/Virtual/Regions/BuddyAllocatedMemoryRegion.h>

#define SLAB_MAX_NAMELEN    40
#define SLAB_HWCACHE_ALIGN  0x20    //FIXME: make processor independent
#define SLAB_MAXORDER       5
#define SLAB_LIMIT          (PAGE_SIZE >> 3)    //if objects are bigger than 1/8th of a page size, they are off-slab

//FIXME: Get a proper place for this helper macro
#define ALIGN_TO_VAL(val, boundary) (( (val) + (boundary) - 1 ) & ~((boundary) - 1))
#define SMP_CACHE_BYTES 0x20
#define SMP_CACHE_ALIGN SMP_CACHE_BYTES
#define L1_CACHE_ALIGN(val)  ( ( (val) + SMP_CACHE_ALIGN - 1) & ~(SMP_CACHE_ALIGN -1 ) )

namespace Memory
{
    class SlabAllocator;
    class SlabCache;
    
    #define SLAB_BUF_END        (((uint32_t)(~0U))-0)
    
    class Slab
    {
        friend class SlabCache;
    private:
        Slab() {}
        ~Slab() {}
        
        SlabCache* cache;
        Address objectStart;
        uint32_t freeIndex;     //Index to next free object in the free-array
        uint32_t inUse;
        
        //Linked list for Slabs in SlabCache
        Slab* Next;
    
        void* AllocateOnSlab();
        bool IsFull() { return this->freeIndex == SLAB_BUF_END; }
        
        //the free-array is directly behind this class, so increment this pointer and cast to array pointer
        inline uint32_t* GetFreeArray() { return (uint32_t*)(this + 1); }
    public:
        
    };
    
    class SlabCache
    {
    friend class SlabAllocator;
    friend class SlabCacheHead;
        
    protected:
        SlabCache(const char* Name, int Align, int Size, SlabAllocator* ParentAllocator) { this->Initialize(Name, Align, Size, ParentAllocator); }
        ~SlabCache() {}
        
        void Initialize(const char* Name, int Align, int Size, SlabAllocator* ParentAllocator);
        
        SlabCache* NextCache;   //pointer to next list item
        
        Slab* fullSlabList;
        Slab* partialSlabList;
        Slab* freeSlabList;
        //SlabCache* slab_pointer;    //Pointer to management object, kept off-slab
        
        SlabAllocator* allocator;
        char name[SLAB_MAX_NAMELEN];
        size_t objSize;
        size_t order;
        size_t objectsPerSlab;
        size_t objectsAllocated;
        size_t objectsActive;
        bool offSlab;
        
        static size_t EstimateNrObjects(size_t order, size_t objSize, size_t* nr, size_t* wastage);
        static size_t GetObjectCount(size_t* objSize, size_t align, size_t* order, size_t* left);
        
        /* grow the cache by one slab */
        Slab* Grow();
        
        /* list management */
        void AddSlabToList(Slab** listHead, Slab* toAppend);
        void RemoveSlabFromList(Slab** listHead, Slab* toRemove);
        
    public:
        /* allocate a new object in a Slab inside this SlabCache */
        void* AllocateObject();
        void FreeObject(void* object);
        
        inline size_t GetObjectSize() { return objSize; }
    };
    
    /* The mother of all SlabCaches */
    class SlabCacheHead : public SlabCache
    {
    friend class SlabAllocator;
    
    private:
        SlabCacheHead(SlabAllocator* ParentAllocator);
        ~SlabCacheHead() {}
        
        /* to be called internally by SlabAllocator */
        SlabCache* CreateNewCache(const char* cacheName, size_t objectSize, size_t alignment);
    };
    
    
    class SlabAllocator : public BuddyAllocatedMemoryRegion
    {
    private:
        SlabCacheHead* cacheHead;
	
    public:
        SlabAllocator(Address RegionStart, size_t RegionSize);
        ~SlabAllocator() {}
        
        SlabCache* CreateCache(const char* cacheName, size_t objectSize, size_t alignment);
        void DestroyCache(SlabCache* cache);
    };
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
