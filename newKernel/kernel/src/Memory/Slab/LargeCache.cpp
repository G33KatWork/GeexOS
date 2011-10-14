#include <kernel/Memory/Slab/LargeCache.h>
#include <kernel/debug.h>
#include <string.h>
#include <stdlib.h>

using namespace Memory::Slab;
using namespace Debug;

void LargeCache::Destroy()
{
    SlabCache::Destroy();
    
    SLAB_LARGE_DEBUG_MSG("Destroying Hashtable");
    hashTable.~BHashTable();
}

void* LargeCache::AllocateObject()
{
    HashedSlab* slab = (HashedSlab*)this->GetNonEmptySlab();
    
    ASSERT(slab->magic_valid == SLAB_MAGIC_VALID, "Slabs magic value doesn't indicate it's valid");
    
    if(slab->IsEmpty())
    {
        this->freeSlabList.Remove(slab);
        this->partialSlabList.Prepend(slab);
    }
    
    SLAB_LARGE_DEBUG_MSG("Allocating off-slab object from Slab living at " << hex << (Address)slab);
    
    void* allocatedObject = (void*)(slab->objectStart + slab->freeIndex * this->objSize);
    slab->freeIndex = this->GetFreeArray(slab)[slab->freeIndex];
    
    slab->inUse++;
    
    if(slab->IsFull())
    {
        SLAB_LARGE_DEBUG_MSG("Slab is now full, moving into full-list");
        this->partialSlabList.Remove(slab);
        this->fullSlabList.Prepend(slab);
    }
    
    objectsAllocated++;
    return allocatedObject;
}

void LargeCache::FreeObject(void* object)
{
    SLAB_LARGE_DEBUG_MSG("Freeing object at " << hex << (Address)object << " in LargeCache " << name);
    
    //get slab belonging to object
    HashedSlab* slab = hashTable.Lookup(LowerBoundary((Address)object, 1 << PAGE_SHIFT << this->order));
    SLAB_SMALL_DEBUG_MSG("Slab seems to be at " << hex << (Address)slab);
    
    ASSERT(slab->magic_valid == SLAB_MAGIC_VALID, "Slabs magic value doesn't indicate it's valid");
    
    if(!slab)
        PANIC("Trying to free object which doesn't belong to this LargeCache with name " << name);
    
    if(slab->IsFull())
    {
        this->fullSlabList.Remove(slab);
        this->partialSlabList.Prepend(slab);
    }
    
    uint32_t objectIndex = (((Address)object) - slab->objectStart) / this->objSize;
    SLAB_LARGE_DEBUG_MSG("Index of object seems to be " << dec << objectIndex);
    
    GetFreeArray(slab)[objectIndex] = slab->freeIndex;
    slab->freeIndex = objectIndex;
    slab->inUse--;
    
    if(slab->IsEmpty())
    {
        this->partialSlabList.Remove(slab);
        this->freeSlabList.Prepend(slab);
    }
    
    ResizeHashTableIfNeeded();
    objectsAllocated--;
}

Slab* LargeCache::Grow()
{
    SLAB_LARGE_DEBUG_MSG("Growing LargeCache " << this->name << " by one Slab with order " << dec << this->order);
    
    void* newSlabBuffer = AllocateFromSizeSlabs(sizeof(HashedSlab));
    
    SLAB_LARGE_DEBUG_MSG("Initializing off-slab Slab at " << hex << (Address)newSlabBuffer);
    
    HashedSlab* slab = new(newSlabBuffer) HashedSlab();
    SLAB_LARGE_DEBUG_MSG("HashedSlab object lives at " << hex << (Address)slab);
    slab->inUse = 0;
    slab->freeIndex = 0;
    slab->cache = this;
    slab->magic_valid = SLAB_MAGIC_VALID;
    slab->objectStart = allocator->AllocateBuddy(this->order);
    slab->freeArray = (uint32_t*)AllocateFromSizeSlabs(objectsPerSlab * sizeof(uint32_t));
    SLAB_LARGE_DEBUG_MSG("Objects in Slab start at " << hex << slab->objectStart);
    
    for(uint32_t i = 0; i < objectsPerSlab - 1; i++)
        this->GetFreeArray(slab)[i] = i+1;
    this->GetFreeArray(slab)[objectsPerSlab - 1] = SLAB_BUF_END;
    
    this->freeSlabList.Prepend(slab);
    
    //Add slab to hashTable for lookup of objects
    hashTable.Insert(slab);
    ResizeHashTableIfNeeded();
    
    return slab;
}

void LargeCache::ReleaseSlab(Slab* slab)
{
    SLAB_LARGE_DEBUG_MSG("Releasing HashedSlab at " << hex <<(Address)slab << " from " << this->name);
    
    ASSERT(slab->magic_valid == SLAB_MAGIC_VALID, "Slabs magic value doesn't indicate it's valid");
    slab->magic_valid = 0x0;
    
    this->hashTable.Remove((HashedSlab*)slab);
    FreeFromSizeSlabs(((HashedSlab*)slab)->freeArray);
    this->allocator->FreeBuddy(slab->objectStart, this->order);
    FreeFromSizeSlabs(slab);
    
    this->ResizeHashTableIfNeeded();
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