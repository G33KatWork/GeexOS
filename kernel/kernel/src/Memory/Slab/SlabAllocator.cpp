#include <kernel/Memory/Slab/SlabAllocator.h>
#include <kernel/Memory/Slab/SmallCache.h>
#include <kernel/Memory/Slab/LargeCache.h>
#include <kernel/debug.h>
#include <string.h>
#include <stdlib.h>

using namespace Memory::Slab;
using namespace Debug;

struct cacheSizes
{
    size_t size;
    SlabCache* cache;
};

static struct cacheSizes cacheSizes[] = {
    { 1 << 5, NULL },
    { 1 << 6, NULL },
    { 1 << 7, NULL },
    { 1 << 8, NULL },
    { 1 << 9, NULL },
    { 1 << 10, NULL },
    { 1 << 11, NULL },
    { 1 << 12, NULL },
    { 1 << 13, NULL },
    { 0,      NULL }
};

void Memory::Slab::InitializeSizeCaches(SlabAllocator* allocator)
{
    for(int i = 0; cacheSizes[i].size; i++)
    {
        char cacheName[SLAB_MAX_NAMELEN] = "size-";
        char sizeStr[10] = {0};
        itoa(cacheSizes[i].size, sizeStr, 10);
        strncat(cacheName, sizeStr, SLAB_MAX_NAMELEN);
        
        SlabCache* cache = allocator->CreateCache(cacheName, cacheSizes[i].size, 0);
        if(!cache)
            PANIC("SlabCache for objects with size " << dec << cacheSizes[i].size << " could not be created");
        
        cacheSizes[i].cache = cache;
    }
}

void* Memory::Slab::AllocateFromSizeSlabs(size_t size)
{
    SLAB_ALLOCATOR_DEBUG_MSG("Allocating object from size caches for object size " << dec << size);
    
    //Need to save a pointer to the SlabCache for freeing later
    //FIXME: Is this hacky?
    size_t totalSize = size + sizeof(SlabCache*);
    
    int i = 0;
    while(cacheSizes[i].size != 0 && totalSize > cacheSizes[i].size)
        i++;
    
    if(cacheSizes[i].size == 0 || cacheSizes[i].cache == NULL)
    {
        PANIC("No suitable cache size for object with size " << dec << size << " found");
        return NULL;
    }
    else
    {
        SlabCache** object = (SlabCache**)cacheSizes[i].cache->AllocateObject();
        *object = cacheSizes[i].cache;
        object++;
        return (void*)object;
    }
}

void Memory::Slab::FreeFromSizeSlabs(void* address)
{
    //TODO: Sanity checks
    SlabCache** object = (SlabCache**)address;
    object--;
    SlabCache* cache = *object;
    cache->FreeObject((void*)object);
}

SlabAllocator::SlabAllocator(Address RegionStart, size_t RegionSize)
    : BuddyAllocatedMemoryRegion(RegionStart, RegionSize, "SLAB Allocator", PAGE_SHIFT)
{
    SLAB_ALLOCATOR_DEBUG_MSG("Initializing SLAB Allocator...");
    
    cacheHead = new SmallCache("CacheHead", SLAB_HWCACHE_ALIGN, sizeof(SmallCache), this);
}

SlabCache* SlabAllocator::CreateCache(const char* cacheName, size_t objectSize, size_t alignment)
{
    SlabCache* cache = NULL;
    
    SLAB_ALLOCATOR_DEBUG_MSG("Creating a new SLABCache " << cacheName << " with object size " << hex << objectSize << " aligned at " << alignment);
    
    /* check if name present and not too long */
    if(!cacheName || strlen(cacheName) > SLAB_MAX_NAMELEN)
        return NULL;
    
    /* check if object size is too big */
    if(GetOrder(objectSize) > SLAB_MAXORDER)
        return NULL;
    
    /* check if slab with that name already exists */
    for(SlabCache* curCache = this->cacheList.Head(); curCache != NULL; curCache = this->cacheList.GetNext(curCache))
    {
        if(!strcmp(curCache->GetName(), cacheName))
            return NULL;
    }
    
    
    
    if(objectSize > SLAB_LIMIT)    //off-slab?
    {
        void* largeCacheBuffer = AllocateFromSizeSlabs(sizeof(LargeCache));
        if(largeCacheBuffer == NULL)
        {
            SLAB_ALLOCATOR_DEBUG_MSG("Error creating new LargeCache " << cacheName);
            return NULL;
        }
        
        SLAB_ALLOCATOR_DEBUG_MSG("Initializing new LargeCache at " << hex << (Address)largeCacheBuffer);
        cache = new(largeCacheBuffer) LargeCache(cacheName, alignment, objectSize, this);
        
        this->cacheList.Append(cache);
    }
    else
    {
        void* cacheBuffer = cacheHead->AllocateObject();
        if(cacheBuffer == NULL)
        {
            SLAB_ALLOCATOR_DEBUG_MSG("Error creating new SmallSlabCache " << cacheName);
            return NULL;
        }
        
        SLAB_ALLOCATOR_DEBUG_MSG("Initializing new SmallCache at " << hex << (Address)cacheBuffer);
        cache = new(cacheBuffer) SmallCache(cacheName, alignment, objectSize, this);

        this->cacheList.Append(cache);
    }
    
    return cache;
}

void SlabAllocator::DestroyCache(SlabCache* cache)
{
    this->cacheList.Remove(cache);
    
    if(cache->objSize > SLAB_LIMIT)
        FreeFromSizeSlabs(cache);
    
    cache->Destroy();
}

void SlabAllocator::FreeUnusedMemory()
{
    SLAB_ALLOCATOR_DEBUG_MSG("Releasing unused memory");
    for(SlabCache* curCache = this->cacheList.Head(); curCache != NULL; curCache = this->cacheList.GetNext(curCache))
        curCache->ReleaseUnusedMemory();
}

void SlabAllocator::DumpCacheInfo(BaseOutputDevice* c)
{
    for(SlabCache* curCache = cacheList.Head(); curCache != NULL; curCache = cacheList.GetNext(curCache))
    {
        *c << "SLABALLOC: " << "\tName: " << curCache->name << endl;
        *c << "SLABALLOC: " << "\tObjSize: " << dec << (unsigned int)curCache->objSize << endl;
        *c << "SLABALLOC: " << "\tOrder: " << dec << (unsigned int)curCache->order << endl;
        *c << "SLABALLOC: " << "\tObjectsPerSlab: " << dec << (unsigned int)curCache->objectsPerSlab << endl;
        *c << "SLABALLOC: " << "\tObjectsAllocated: " << dec << (unsigned int)curCache->objectsAllocated << endl;
        *c << endl;
    }
}
