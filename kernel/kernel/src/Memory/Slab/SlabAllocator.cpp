#include <kernel/Memory/Slab/SlabAllocator.h>
#include <kernel/debug.h>
#include <string.h>

using namespace Memory;
using namespace Debug;

SlabAllocator::SlabAllocator(Address RegionStart, size_t RegionSize)
    : BuddyAllocatedMemoryRegion(RegionStart, RegionSize, "SLAB Allocator", PAGE_SHIFT)
{
    SLAB_DEBUG_MSG("Initializing SLAB Allocator...");
    
    cacheHead = new SlabCache("CacheHead", SLAB_HWCACHE_ALIGN, sizeof(SlabCache), this);
}

SlabCache::SlabCache(const char* Name, int Align, int Size, SlabAllocator* ParentAllocator)
{
    objSize = Size;
    int align = Align;
    
    size_t wastage = 0;
    objectsPerSlab = SlabCache::GetObjectCount(&objSize, align, &order, &wastage);
    if(!objectsPerSlab || order > SLAB_MAXORDER)
        PANIC("Unable to initialize SLAB cache " << Name);
    
    //FIMXE: objSize already with alignment?
    offSlab = objSize > SLAB_LIMIT;
    
    strcpy(name, Name);
    allocator = ParentAllocator;
    objectsAllocated = 0;
    objectsActive = 0;
}

SlabCache* SlabAllocator::CreateCache(const char* cacheName, size_t objectSize, size_t alignment)
{
    SlabCache* cache = NULL;
    
    SLAB_DEBUG_MSG("Creating a new SLABCache " << cacheName << " with object size " << hex << objectSize << " aligned at " << alignment);
    
    /* check if name present and not too long */
    if(!cacheName || strlen(cacheName) > SLAB_MAX_NAMELEN)
        return NULL;
    
    /* check if object size is too big */
    if(GetOrder(objectSize) > SLAB_MAXORDER)
        return NULL;
    
    /* check if slab with that name already exists */
    for(SlabCache* curCache = this->cacheList.Head(); curCache != NULL; curCache = this->cacheList.GetNext(curCache))
    {
        if(!strcmp(curCache->name, cacheName))
            return NULL;
    }
    
    void* cacheBuffer = cacheHead->AllocateObject();
    if(cacheBuffer == NULL)
    {
        SLAB_DEBUG_MSG("Error creating new SlabCache " << cacheName);
        return NULL;
    }
    
    SLAB_DEBUG_MSG("Initializing new Cache at " << hex << (Address)cacheBuffer);
    cache = new(cacheBuffer) SlabCache(cacheName, alignment, objectSize, this);
    
    this->cacheList.Append(cache);
    
    return cache;
}

void* SlabCache::AllocateObject()
{
    Slab* slab = NULL;
    
    /* partially used slabs available? */
    if(this->partialSlabList.IsEmpty())
    {
        SLAB_DEBUG_MSG("No more partial free Slabs in SlabCache " << this->name);
        
        //free slabs available?
        if(this->freeSlabList.IsEmpty())
        {
            SLAB_DEBUG_MSG("No more free Slabs in SlabCache " << this->name << ". Need to grow");
            
            //neither partial used or free slabs available, expand with a new
            slab = this->Grow();
            if(!slab)
            {
                SLAB_DEBUG_MSG("Error while growing SlabCache " << this->name);
                return NULL;
            }
        }
        else    //use first free slab
            slab = this->freeSlabList.Head();
        
        //since we are directly going to allocate stuff in the slab,
        //remove it from free list and add it to partially used list
        this->freeSlabList.Remove(slab);
        this->partialSlabList.Prepend(slab);
    }
    else    //use first partial used slab
        slab = this->partialSlabList.Head();
    
    void* allocatedObject = NULL;
    
    if(offSlab)
    {
        PANIC("Off-slab not supported yet");
    }
    else
        allocatedObject = slab->AllocateOnSlab();
    
    if(slab->IsFull())
    {
        SLAB_DEBUG_MSG("Slab is now full, moving into full-list");
        this->partialSlabList.Remove(slab);
        this->fullSlabList.Prepend(slab);
    }
    
    return allocatedObject;
}

void FreeObject(void* object)
{
    
}

void* Slab::AllocateOnSlab()
{
    SLAB_DEBUG_MSG("Allocating on-slab object from Slab living at " << hex << (Address)this);
    
    if(IsFull())
    {
        PANIC("Cannot allocate an object in a full Slab");
    }
    
    void* object = (void*)(objectStart + this->freeIndex * this->cache->GetObjectSize());
    this->freeIndex = GetFreeArray()[this->freeIndex];
    
    this->inUse++;
    
    return object;
}

Slab* SlabCache::Grow()
{
    SLAB_DEBUG_MSG("Growing SlabCache " << this->name << " by one Slab with order " << dec << this->order);
    
    Address newSlab = allocator->AllocateBuddy(this->order);
    
    //TODO: Handle off-slab
    Slab* slab = NULL;
    
    if(offSlab)
    {
        PANIC("No off-slabs yet");
    }
    else
    {
        SLAB_DEBUG_MSG("Initializing on-slab Slab at " << hex << newSlab);
        
        slab = new((void*)newSlab) Slab();
        SLAB_DEBUG_MSG("Slab object lives at " << hex << (Address)slab);
        slab->inUse = 0;
        slab->freeIndex = 0;
        slab->cache = this;
        slab->objectStart = (newSlab + (1 << order << PAGE_SHIFT)) - (objSize * objectsPerSlab);
        SLAB_DEBUG_MSG("Objects in Slab start at " << hex << slab->objectStart);
        
        //size_t freeArraySize = this->objectsPerSlab * sizeof(uint32_t);     //Array of free slabs, each entry is one uint32_t
        
        for(uint32_t i = 0; i < objectsPerSlab - 1; i++)
            slab->GetFreeArray()[i] = i+1;
        slab->GetFreeArray()[objectsPerSlab - 1] = SLAB_BUF_END;
    }
    
    this->freeSlabList.Prepend(slab);
    return slab;
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
    size_t headSize = 0;
    size_t total = (1 << order) << PAGE_SHIFT;
    
    if(objSize < SLAB_LIMIT)
    {
        //if cache is on-slab, we need to add the Slab structure
        headSize = L1_CACHE_ALIGN(sizeof(Slab));
    }
    
    while((i*objSize + headSize) <= total)
        i++;
    
    if(i)
        i--;
    
    *wastage = total - (i*objSize + headSize);
    *nr = i;
    
    SLAB_DEBUG_MSG("Estimated object count. Order: " << order << " ObjectSize: " << objSize << " Nr: " << i << " Wastage: " << *wastage);
    return i;
}
