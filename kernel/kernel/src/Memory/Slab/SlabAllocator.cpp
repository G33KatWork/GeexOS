#include <kernel/Memory/Slab/SlabAllocator.h>
#include <kernel/debug.h>
#include <string.h>

using namespace Memory;

SlabAllocator::SlabAllocator()
{
    SLAB_DEBUG_MSG("Initializing SLAB Allocator...");
    
    InitializeCacheHead();
    InitializeCacheSizes();
}

SlabCache* SlabAllocator::CreateCache(char* cacheName, size_t objectSize, size_t alignment)
{
    SlabCache* cache = NULL;
    size_t nr = 0, wastage = 0, order = 0, slab_size = 0;
    
    if(!cacheName || strlen(cacheName) > SLAB_MAX_NAMELEN)
        return NULL;
    
    if(GetOrder(objectSize) > SLAB_MAXORDER)
        return NULL;
    
    for(SlabCache* curCache = cacheHead.Next; curCache != NULL; curCache = curCache->Next)
    {
        if(!strcmp(curCache->name, cacheName))
            return NULL;
    }
    
    nr = GetObjectCount(&objectSize, alignment, &order, &wastage);
    if(!nr || order > SLAB_MAXORDER)
        return NULL;
    
    cache = AllocateCache(&cacheHead);
    
    return NULL;
}

void SlabAllocator::DestroyCache(SlabCache* cache)
{
    
}

void SlabAllocator::InitializeCacheHead()
{
    size_t size = sizeof(SlabCache);
    int align = SLAB_HWCACHE_ALIGN;
    
    size_t nr = 0, wastage = 0, order = 0;
    nr = GetObjectCount(&size, align, &order, &wastage);
    if(!nr || order > SLAB_MAXORDER)
        PANIC("Unable to initialize SLAB cache head");
    
    memset(&cacheHead, 0, sizeof(SlabCache));
    
    cacheHead.nr_objects = nr;
    cacheHead.size = size;
    cacheHead.order = order;
}

void SlabAllocator::InitializeCacheSizes()
{
    
}

size_t SlabAllocator::GetObjectCount(size_t* objSize, size_t align, size_t* order, size_t* left)
{   
    size_t ord = 0;
    size_t nr = 0, wastage = 0, size = *objSize;

    SLAB_DEBUG_MSG("Geting object count for a new slab. ObjectSize: " << size << " Alignment: " << align);
    
    while(size < align/2)
        align /= 2;
    
    if(align)
        size = ALIGN_TO_VAL(size, align);
    
    do
    {
        EstimateNrObjects(ord, size, &nr, &wastage);
        if(!nr)
            continue;
        
        //wastage of 1/8th of the slab size is acceptable
        if((wastage * 8) > (1 << ord) << PAGE_SHIFT)
        {
            SLAB_DEBUG_MSG("Wastage in slab is high. Size: " << (unsigned)size << " Order: " << (unsigned)order);
        }
        
        break;
    } while(++ord <= SLAB_MAXORDER);
    
    *order = ord;
    *left = wastage;
    *objSize = size;
    
    SLAB_DEBUG_MSG("Object count for slab with order " << (unsigned)ord << " wastage " << (unsigned)wastage << " and size " << (unsigned)size << " is " << (unsigned)nr);
    return nr;
}

size_t SlabAllocator::EstimateNrObjects(size_t order, size_t objSize, size_t* nr, size_t* wastage)
{
    int i = 0;
    size_t extra = 0, base = 0;
    size_t ctrl_data = 2 * sizeof(void*);
    size_t total = (1 << order) << PAGE_SHIFT;
    
    if(objSize < SLAB_LIMIT)
    {
        base = sizeof(SlabCache);
        extra = sizeof(int);
    }
    
    while((i*objSize + L1_CACHE_ALIGN(base + i*extra + ctrl_data)) <= total)
        ++i;
    
    if(i)
        --i;
    
    *wastage = total - (i*objSize + L1_CACHE_ALIGN(base + i*extra + ctrl_data));
    *nr = i;
    
    SLAB_DEBUG_MSG("Estimated object count. Order: " << order << " ObjectSize: " << objSize << " Nr: " << i << " Wastage: " << *wastage);
    return i;
}

SlabCache* SlabAllocator::AllocateCache(SlabCache* slabCache)
{
    if(slabCache->partial_list == NULL)
    {
        if(slabCache->free_list == NULL)
        {
            
        }
    }
}

Slab* SlabAllocator::CacheGrow(SlabCache* slabCache)
{
    Slab* newSlab = NULL;
    //TODO: get some new space from region and add new slab to free_list in given slabCache
}
