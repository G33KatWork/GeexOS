#include <kernel/Memory/Slab/SlabAllocator.h>
#include <kernel/debug.h>
#include <string.h>

using namespace Memory;

SlabAllocator::SlabAllocator(Address RegionStart, size_t RegionSize)
    : LazyMemoryRegion(RegionStart, RegionSize, "SLAB Allocator", ALLOCFLAG_WRITABLE)
{
    SLAB_DEBUG_MSG("Initializing SLAB Allocator...");
    
    //cacheHead = SlabCacheHead();
}

SlabCacheHead::SlabCacheHead()
{
    size = sizeof(SlabCache);
    int align = SLAB_HWCACHE_ALIGN;
    
    size_t wastage = 0;
    nr_objects = SlabCache::GetObjectCount(&size, align, &order, &wastage);
    if(!nr_objects || order > SLAB_MAXORDER)
        PANIC("Unable to initialize SLAB cache head");
    
    strcpy(name, "CacheHead");
    NextCache = NULL;
    full_list = NULL;
    partial_list = NULL;
    free_list = NULL;
    nr_allocated = 0;
    nr_active = 0;
}

SlabCache* SlabCacheHead::CreateNewCache(const char* cacheName, size_t objectSize, size_t alignment)
{
    SlabCache* cache = NULL;
    size_t nr = 0, wastage = 0, newCacheOrder = 0, slab_size = 0;
    
    /* check if name present and not too long */
    if(!cacheName || strlen(cacheName) > SLAB_MAX_NAMELEN)
        return NULL;
    
    /* check if object size is too big */
    if(GetOrder(objectSize) > SLAB_MAXORDER)
        return NULL;
    
    /* check if slab with that name already exists */
    for(SlabCache* curCache = this->NextCache; curCache != NULL; curCache = curCache->NextCache)
    {
        if(!strcmp(curCache->name, cacheName))
            return NULL;
    }
    
    /* calculate object count */
    nr = SlabCache::GetObjectCount(&objectSize, alignment, &newCacheOrder, &wastage);
    if(!nr || newCacheOrder > SLAB_MAXORDER)
        return NULL;
    
    /* Allocate a new cache as an object in us (the cache head) */
    cache = (SlabCache*)AllocateObject();
    
    //TODO: Initialize cache head, initialize it to be empty and add it to slab list in cache head
    
    return NULL;
}

SlabCache* SlabAllocator::CreateCache(const char* cacheName, size_t objectSize, size_t alignment)
{
    return cacheHead.CreateNewCache(cacheName, objectSize, alignment);
}

void* SlabCache::AllocateObject()
{
    Slab* slab = NULL;
    
    /* partially used slabs available? */
    if(this->partial_list == NULL)
    {
        SLAB_DEBUG_MSG("No more partial free Slabs in SlabCache " << this->name);
        
        //free slabs available?
        if(this->free_list == NULL)
        {
            SLAB_DEBUG_MSG("No more free Slabs in SlabCache " << this->name << ". Need to grow");
            
            //neither partial used or free slabs available, expand with a new
            slab = this->Grow();
            if(!slab)
            {
                SLAB_DEBUG_MSG("Error while growing SlabCache " << this->name);
                return NULL;
            }
            
            //since we are directly going to allocate stuff in the slab,
            //remove it from free list and add it to partially used list
            RemoveSlabFromList(&this->free_list, slab);
            AddSlabToList(&this->partial_list, slab);
        }
        else    //use first free slab
            slab = this->free_list;
    }
    else    //use first partial used slab
        slab = this->partial_list;
    
    return slab->Allocate();
}

void* Slab::Allocate()
{
    return NULL;
}

Slab* SlabCache::Grow()
{
    SLAB_DEBUG_MSG("Growing SlabCache " << this->name << " by one Slab");
    
    Slab* newSlab = NULL;
    return newSlab;
    //TODO: get some new space from region and add new slab to free_list in given slabCache
}

void SlabCache::AddSlabToList(Slab** listHead, Slab* toAppend)
{
   SLAB_DEBUG_MSG("Adding SLAB to Cache " << this->name);
   toAppend->Next = *listHead;
   *listHead = toAppend;
}

void SlabCache::RemoveSlabFromList(Slab** listHead, Slab* toRemove)
{
   SLAB_DEBUG_MSG("Removing SLAB from Cache " << this->name);
   for(Slab* curSlab = *listHead; curSlab->Next != NULL; curSlab = curSlab->Next)
   {
       if(curSlab->Next != NULL && curSlab->Next == toRemove)
       {
           curSlab->Next = curSlab->Next->Next;
           return;
       }
   }
   
   SLAB_DEBUG_MSG("Slab to remove not found in list");
}









/* calculate slab order, internal fragmentation and number of objects for given object size */
size_t SlabCache::GetObjectCount(size_t* objSize, size_t align, size_t* order, size_t* left)
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
        SlabCache::EstimateNrObjects(ord, size, &nr, &wastage);
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

/* Calculate the number of objects which fit in one Slab in given order with management information and alignment */
size_t SlabCache::EstimateNrObjects(size_t order, size_t objSize, size_t* nr, size_t* wastage)
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
        i++;
    
    if(i)
        i--;
    
    *wastage = total - (i*objSize + L1_CACHE_ALIGN(base + i*extra + ctrl_data));
    *nr = i;
    
    SLAB_DEBUG_MSG("Estimated object count. Order: " << order << " ObjectSize: " << objSize << " Nr: " << i << " Wastage: " << *wastage);
    return i;
}
