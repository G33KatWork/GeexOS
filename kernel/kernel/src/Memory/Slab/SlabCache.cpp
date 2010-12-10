#include <kernel/Memory/Slab/SlabCache.h>
#include <kernel/debug.h>
#include <string.h>
#include <stdlib.h>
#include <arch/HAL.h>

using namespace Memory::Slab;
using namespace Debug;

//FIXME: Get a proper place for this helper macro
#define ALIGN_TO_VAL(val, boundary) (( (val) + (boundary) - 1 ) & ~((boundary) - 1))
#define SMP_CACHE_BYTES 0x20
#define SMP_CACHE_ALIGN SMP_CACHE_BYTES
#define L1_CACHE_ALIGN(val)  ( ( (val) + SMP_CACHE_ALIGN - 1) & ~(SMP_CACHE_ALIGN -1 ) )

SlabCache::SlabCache(const char* Name, int Align, int Size, SlabAllocator* ParentAllocator)
{
    objSize = Size;
    int align = Align;
    
    size_t wastage = 0;
    objectsPerSlab = SlabCache::GetObjectCount(&objSize, align, &order, &wastage);
    if(!objectsPerSlab || order > SLAB_MAXORDER)
        PANIC("Unable to initialize SLAB cache " << Name);
    
    //FIMXE: objSize already with alignment?
    //offSlab = objSize > SLAB_LIMIT;
    
    strcpy(name, Name);
    allocator = ParentAllocator;
    //objectsAllocated = 0;
    //objectsActive = 0;
}

Slab* SlabCache::GetNonEmptySlab()
{
    Slab* slab = NULL;
    
    /* partially used slabs available? */
    if(this->partialSlabList.IsEmpty())
    {
        SLAB_CACHE_DEBUG_MSG("No more partial free Slabs in SlabCache " << this->name);
        
        //free slabs available?
        if(this->freeSlabList.IsEmpty())
        {
            SLAB_CACHE_DEBUG_MSG("No more free Slabs in SlabCache " << this->name << ". Need to grow");
            
            //neither partial used or free slabs available, expand with a new
            slab = this->Grow();
            if(!slab)
            {
                SLAB_CACHE_DEBUG_MSG("Error while growing SlabCache " << this->name);
                return NULL;
            }
        }
        else    //use first free slab
            slab = this->freeSlabList.Head();
    }
    else    //use first partial used slab
        slab = this->partialSlabList.Head();
    
    return slab;
}

/* calculate slab order, internal fragmentation and number of objects for given object size */
size_t SlabCache::GetObjectCount(size_t* objSize, size_t align, size_t* order, size_t* left)
{   
    size_t ord = 0;
    size_t nr = 0, wastage = 0, size = *objSize;

    SLAB_CACHE_DEBUG_MSG("Geting object count for a new slab. ObjectSize: " << size << " Alignment: " << align);
    
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
            SLAB_CACHE_DEBUG_MSG("Wastage in slab is high. Size: " << (unsigned)size << " Order: " << (unsigned)order);
        }
        
        break;
    } while(++ord <= SLAB_MAXORDER);
    
    *order = ord;
    *left = wastage;
    *objSize = size;
    
    SLAB_CACHE_DEBUG_MSG("Object count for slab with order " << (unsigned)ord << " wastage " << (unsigned)wastage << " and size " << (unsigned)size << " is " << (unsigned)nr);
    return nr;
}

void SlabCache::Destroy()
{
    SLAB_CACHE_DEBUG_MSG("Destroying LargeCache at " << hex << (Address)this << " with name " << name);
    
    SLAB_CACHE_DEBUG_MSG("Returning free slabs to buddy allocator");
    for(Slab* curSlab = this->freeSlabList.Head(); curSlab != NULL; curSlab = this->freeSlabList.GetNext(curSlab))
        this->ReleaseSlab(curSlab);
    
    SLAB_CACHE_DEBUG_MSG("Returning partial slabs to buddy allocator");
    for(Slab* curSlab = this->partialSlabList.Head(); curSlab != NULL; curSlab = this->partialSlabList.GetNext(curSlab))
        this->ReleaseSlab(curSlab);
    
    SLAB_CACHE_DEBUG_MSG("Returning full slabs to buddy allocator");
    for(Slab* curSlab = this->fullSlabList.Head(); curSlab != NULL; curSlab = this->fullSlabList.GetNext(curSlab))
        this->ReleaseSlab(curSlab);
}

void SlabCache::ReleaseUnusedMemory()
{
    SLAB_CACHE_DEBUG_MSG("Returning free slabs to buddy allocator from slab " << this->name);
    for(Slab* curSlab = this->freeSlabList.Head(); curSlab != NULL; curSlab = this->freeSlabList.Head())
    {  
        this->freeSlabList.Remove(curSlab);
        this->ReleaseSlab(curSlab);
    }
}

/* Calculate the number of objects which fit in one Slab in given order with management information and alignment */
size_t SlabCache::EstimateNrObjects(size_t order, size_t objSize, size_t* nr, size_t* wastage)
{
    int i = 0;
    size_t headSize = 0;
    size_t freeArraySizePerElement = 0;
    size_t total = (1 << order) << PAGE_SHIFT;
    
    if(objSize < SLAB_LIMIT)
    {
        //if cache is on-slab, we need to add the Slab structure
        headSize = sizeof(Slab);
        
        //and the free array entry per element
        //if it is a large cache, it gets seperately allocated
        //if it is a small cache then it it located directly behin the slab header
        freeArraySizePerElement = sizeof(uint32_t);
    }
    
    while((i*objSize + headSize + freeArraySizePerElement*i) <= total)
        i++;
    
    if(i)
        i--;
    
    *wastage = total - (i*objSize + headSize + freeArraySizePerElement*i);
    *nr = i;
    
    SLAB_CACHE_DEBUG_MSG("Estimated object count. Order: " << order << " ObjectSize: " << objSize << " Nr: " << i << " Wastage: " << *wastage);
    return i;
}
