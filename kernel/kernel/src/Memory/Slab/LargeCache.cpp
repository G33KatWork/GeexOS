#include <kernel/Memory/Slab/LargeCache.h>
#include <kernel/debug.h>
#include <string.h>
#include <stdlib.h>

using namespace Memory::Slab;
using namespace Debug;

void* LargeCache::AllocateObject()
{
    HashedSlab* slab = (HashedSlab*)this->GetNonEmptySlab();
    
    if(slab->IsEmpty())
    {
        this->freeSlabList.Remove(slab);
        this->partialSlabList.Prepend(slab);
    }
    
    SLAB_DEBUG_MSG("Allocating off-slab object from Slab living at " << hex << (Address)slab);
    
    void* allocatedObject = (void*)(slab->objectStart + slab->freeIndex * this->objSize);
    slab->freeIndex = this->GetFreeArray(slab)[slab->freeIndex];
    
    slab->inUse++;
    
    if(slab->IsFull())
    {
        SLAB_DEBUG_MSG("Slab is now full, moving into full-list");
        this->partialSlabList.Remove(slab);
        this->fullSlabList.Prepend(slab);
    }
    
    return allocatedObject;
}

void LargeCache::FreeObject(void* object)
{
    
}

Slab* LargeCache::Grow()
{
    SLAB_DEBUG_MSG("Growing LargeCache " << this->name << " by one Slab with order " << dec << this->order);
    
    void* newSlabBuffer = AllocateFromSizeSlabs(sizeof(HashedSlab));
    
    SLAB_DEBUG_MSG("Initializing off-slab Slab at " << hex << (Address)newSlabBuffer);
    
    HashedSlab* slab = new(newSlabBuffer) HashedSlab();
    SLAB_DEBUG_MSG("HashedSlab object lives at " << hex << (Address)slab);
    slab->inUse = 0;
    slab->freeIndex = 0;
    slab->cache = this;
    slab->objectStart = allocator->AllocateBuddy(this->order);
    slab->freeArray = (uint32_t*)AllocateFromSizeSlabs(objectsPerSlab * sizeof(uint32_t));
    SLAB_DEBUG_MSG("Objects in Slab start at " << hex << slab->objectStart);
    
    for(uint32_t i = 0; i < objectsPerSlab - 1; i++)
        this->GetFreeArray(slab)[i] = i+1;
    this->GetFreeArray(slab)[objectsPerSlab - 1] = SLAB_BUF_END;
    
    this->freeSlabList.Prepend(slab);
    
    //Add slab to hashTable for lookup of objects
    hashTable.Insert(slab);
    ResizeHashTableIfNeeded();
    
    return slab;
}

void LargeCache::ResizeHashTableIfNeeded()
{
    size_t hashSize = hashTable.ResizeNeeded();
    if(hashSize != 0)
    {
        void* newBuffer = AllocateFromSizeSlabs(hashSize);
        if(newBuffer)
        {
            if(hashTable.ResizeNeeded() == hashSize)
            {
                void* oldTable;
                hashTable.Resize(newBuffer, hashSize, true, &oldTable);
                FreeFromSizeSlabs(oldTable);
            }
        }
    }
}