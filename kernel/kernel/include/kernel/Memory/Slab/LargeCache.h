#ifndef _SLAB_ALLOCATOR_LARGECACHE_H_
#define _SLAB_ALLOCATOR_LARGECACHE_H_

#include <arch/types.h>
#include <types.h>
#include <arch/HAL.h>
#include <kernel/DataStructures/HashTable.h>
#include <kernel/Memory/Slab/Slab.h>
#include <kernel/Memory/Slab/SlabCache.h>
#include <kernel/Memory/Slab/SlabAllocator.h>

namespace Memory
{
    namespace Slab
    {
        struct HashedSlab : public Slab
        {
            uint32_t* freeArray;
            struct HashedSlab* HashTableNext;
        };
    
    
    
        class LargeCache : public SlabCache
        {
        private:
            struct HashTableDefinition
            {
                typedef const Address   KeyType;
                typedef HashedSlab      ValueType;
            
                HashTableDefinition(LargeCache* cacheParent) { parent = cacheParent; }
                HashTableDefinition(const HashTableDefinition& definition) { parent = definition.parent; }
            
                size_t HashKey(const Address key) const
                {
                    size_t retKey = LowerBoundary(key, 1 << PAGE_SHIFT << parent->GetOrder());
                    retKey = retKey >> parent->order >> PAGE_SHIFT;
                    
                    SLAB_DEBUG_MSG("HashTable calculated key: " << Debug::hex << retKey << " from " << key);
                    return retKey;
                }
                size_t Hash(HashedSlab* value) const { return HashKey(value->objectStart); }
                bool Compare(const Address key, HashedSlab* value) const { return value->objectStart == key; }
                HashedSlab*& GetLink(HashedSlab* value) const { return value->HashTableNext; }

                LargeCache* parent;
            };
        
            struct SlabHashTableAllocator
            {
                void* Allocate(size_t size) { SLAB_DEBUG_MSG("Allocating for HashTable: " << Debug::dec << size); return AllocateFromSizeSlabs(size); }
                void Free(void* address) { SLAB_DEBUG_MSG("Deallocating for HashTable: " << Debug::dec << (Address)address); return FreeFromSizeSlabs(address); }
            };
        
            typedef DataStructures::BHashTable<HashTableDefinition, false, false, SlabHashTableAllocator>   HashTable;
            friend class HashTableDefinition;
            
            inline uint32_t* GetFreeArray(HashedSlab* slab) { return slab->freeArray; }
    
        protected:
            virtual Slab* Grow();

        public:
            LargeCache(const char* Name, int Align, int Size, SlabAllocator* ParentAllocator)
                : SlabCache(Name, Align, Size, ParentAllocator), hashTable(HashTableDefinition(this))
            {
                //TODO: Init hashTable
            }
            ~LargeCache() {}
        
            virtual void* AllocateObject();
            virtual void FreeObject(void* object);
    
        private:
            void ResizeHashTableIfNeeded();
            static inline Address LowerBoundary(Address addr, size_t size) { return (addr & ~(size - 1)); }
            
            HashTable hashTable;
        };
    }
}
#endif
