#include <kernel/Memory/Slab/SmallCache.h>
#include <kernel/Memory/Slab/SlabAllocator.h>
#include <kernel/debug.h>
#include <string.h>
#include <stdlib.h>

using namespace Memory::Slab;
using namespace Debug;

void* SmallCache::AllocateObject()
{
    Slab* slab = this->GetNonEmptySlab();
    
    ASSERT(slab->magic_valid == SLAB_MAGIC_VALID, "Slabs magic value doesn't indicate it's valid");
    
    if(slab->IsEmpty())
    {
        this->freeSlabList.Remove(slab);
        this->partialSlabList.Prepend(slab);
    }
    
    SLAB_SMALL_DEBUG_MSG("Allocating on-slab object from Slab living at " << hex << (Address)slab);
    
    void* allocatedObject = (void*)(slab->objectStart + slab->freeIndex * this->objSize);
    slab->freeIndex = this->GetFreeArray(slab)[slab->freeIndex];
    
    slab->inUse++;
    
    if(slab->IsFull())
    {
        SLAB_SMALL_DEBUG_MSG("Slab is now full, moving into full-list");
        this->partialSlabList.Remove(slab);
        this->fullSlabList.Prepend(slab);
    }
    
    return allocatedObject;
}

void SmallCache::FreeObject(void* object)
{
    SLAB_SMALL_DEBUG_MSG("Freeing object at " << hex << (Address)object << " in SmallCache " << name);
    
    //get slab belonging to object
    Slab* slab = (Slab*)(((Address)object) & PAGEALIGN_MASK);
    SLAB_SMALL_DEBUG_MSG("Slab seems to be at " << hex << (Address)slab);
    
    ASSERT(slab->magic_valid == SLAB_MAGIC_VALID, "Slabs magic value doesn't indicate it's valid");
    
    if(slab->IsFull())
    {
        this->fullSlabList.Remove(slab);
        this->partialSlabList.Prepend(slab);
    }
    
    uint32_t objectIndex = (((Address)object) - slab->objectStart) / this->objSize;
    SLAB_SMALL_DEBUG_MSG("Index of object seems to be " << dec << objectIndex);
    
    GetFreeArray(slab)[objectIndex] = slab->freeIndex;
    slab->freeIndex = objectIndex;
    slab->inUse--;
    
    if(slab->IsEmpty())
    {
        this->partialSlabList.Remove(slab);
        this->freeSlabList.Prepend(slab);
    }
}

void SmallCache::ReleaseSlab(Slab* slab)
{
    SLAB_SMALL_DEBUG_MSG("Releasing Slab at " << hex <<(Address)slab << " from " << this->name);
    
    ASSERT(slab->magic_valid == SLAB_MAGIC_VALID, "Slabs magic value doesn't indicate it's valid");
    slab->magic_valid = 0x0;
    
    this->allocator->FreeBuddy((Address)slab, this->order);
}

Slab* SmallCache::Grow()
{
    SLAB_SMALL_DEBUG_MSG("Growing SmallCache " << this->name << " by one Slab with order " << dec << this->order);
    
    Address newSlab = allocator->AllocateBuddy(this->order);
    
    SLAB_SMALL_DEBUG_MSG("Initializing on-slab Slab at " << hex << newSlab);
    
    Slab* slab = new((void*)newSlab) Slab();
    SLAB_SMALL_DEBUG_MSG("Slab object lives at " << hex << (Address)slab);
    slab->inUse = 0;
    slab->freeIndex = 0;
    slab->cache = this;
    slab->magic_valid = SLAB_MAGIC_VALID;
    slab->objectStart = (newSlab + (1 << order << PAGE_SHIFT)) - (objSize * objectsPerSlab);
    SLAB_SMALL_DEBUG_MSG("Objects in Slab start at " << hex << slab->objectStart);
    
    for(uint32_t i = 0; i < objectsPerSlab - 1; i++)
        this->GetFreeArray(slab)[i] = i+1;
    this->GetFreeArray(slab)[objectsPerSlab - 1] = SLAB_BUF_END;
    
    this->freeSlabList.Prepend(slab);
    return slab;
}
