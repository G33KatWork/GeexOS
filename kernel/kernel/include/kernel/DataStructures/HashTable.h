#ifndef DATASTRUCTURES_HASHTABLE_H_
#define DATASTRUCTURES_HASHTABLE_H_

#include <types.h>
#include <kernel/global.h>

//TODO: Implement iterators
// Iterator with methods: HashNext(), Next() and Rewind()

/* Hashtable definitions look like this:

struct HashTableDefinition
{
    typedef <Key Type>      KeyType;
    typedef <Value type>    ValueType;

    HashTableDefinition() {}
    HashTableDefinition() {}

    size_t HashKey(KeyType key) const
    {
        //Calculate hash here and return
    }

    size_t Hash(ValueType* value) const { return HashKey(<value to hash>); }
    bool Compare(KeyType key, ValueType* value) const { <compare here> }
    ValueType*& GetLink(ValueType* value) const { <return next pointer for linked list> }
};*/

namespace DataStructures
{
    struct KMallocAllocator
    {
        void* Allocate(size_t size) const
        {
            return kmalloc(size);
        }
        
        void Free(void* address) const
        {
            kfree(address);
        }
    };
    
    template<typename Definition, bool AutoExpand = true, bool CheckDuplicates = false, typename Allocator = KMallocAllocator>
    class BHashTable
    {
    public:
        typedef BHashTable<Definition, AutoExpand, CheckDuplicates, Allocator>  HashTable;
        typedef typename Definition::KeyType     KeyType;
        typedef typename Definition::ValueType   ValueType;
        
        static const size_t kMinimumSize = 8;
        
        BHashTable()
            : curTableSize(0),
            curItemCount(0),
            storage(NULL)
        { }
        
        BHashTable(const Definition& def)
            : definition(def),
            curTableSize(0),
            curItemCount(0),
            storage(NULL)
        { }
        
        BHashTable(const Definition& def, const Allocator& alloc)
            : definition(def),
            allocator(alloc),
            curTableSize(0),
            curItemCount(0),
            storage(NULL)
        { }
        
        ~BHashTable()
        {
            if(storage)
                allocator.Free(storage);
        }
        
        size_t GetTableSize() const { return curTableSize; }
        size_t GetItemCount() const { return curItemCount; }
        
        ValueType* Lookup(const KeyType& key) const
        {
            if(curTableSize == 0)
                return NULL;
            
            size_t index = definition.HashKey(key) & (curTableSize - 1);
            ValueType* slot = storage[index];
            
            while(slot)
            {
                if(definition.Compare(key, slot))
                    break;
                
                slot = _Link(slot);
            }
            
            return slot;
        }
        
        bool Insert(ValueType* value)
        {
            if(curTableSize == 0)
            {
                if(!_Resize(kMinimumSize))
                    return false;
            }
            else if(AutoExpand && curItemCount >= (curTableSize * 200 / 256))
                _Resize(curTableSize * 2);
            
            if(CheckDuplicates && _ExhaustiveSearch(value))
                PANIC("Value to insert in HashTable already exists");
            
            _Insert(storage, curTableSize, value);
            curItemCount++;
            
            return true;
        }
        
        bool Remove(ValueType* value)
        {
            size_t index = definition.Hash(value) & (curTableSize - 1);
            ValueType* previous = NULL;
            ValueType* slot = storage[index];
            
            while(slot)
            {
                ValueType* next = _Link(slot);
                
                if(value == slot)
                {
                    if(previous)
                        _Link(previous) = next;
                    else
                        storage[index] = next;
                    
                    break;
                }
                
                previous = slot;
                slot = next;
            }
            
            if(!slot)
                return false;
            
            if(CheckDuplicates && _ExhaustiveSearch(value))
                PANIC("Value to be removed from HashTable still exists a second time in this table");
            
            curItemCount--;
            
            if(AutoExpand && curTableSize > kMinimumSize && curItemCount < (curTableSize * 50 / 256))
                _Resize(curTableSize / 2);
            
            return true;
        }
        
        ValueType* Clear(bool returnElements = false)
        {
            if(curItemCount == 0)
                return NULL;
            
            ValueType* result = NULL;
            
            if(returnElements)
            {
                ValueType** nextPointer = &result;
                
                for(size_t i = 0; i < curTableSize; i++)
                {
                    ValueType* element = storage[i];
                    if(element)
                    {
                        *nextPointer = element;
                        
                        while(element)
                        {
                            nextPointer = &_Link(element);
                            element = *nextPointer;
                        }
                    }
                }
            }
            
            memset(this->storage, 0, sizeof(ValueType*) * curTableSize);
            
            return result;
        }
        
        size_t ResizeNeeded() const
        {
            size_t size = curTableSize;
            
            if(size == 0 || curItemCount >= (size * 200 / 256))
            {
                if(size == 0)
                    size = kMinimumSize;
                
                while(curItemCount >= size * 200 / 256)
                    size <<= 1;
            }
            else if(size > kMinimumSize && curItemCount < size * 50 / 256)
            {
                while(curItemCount < size * 50 / 256)
                    size >>= 1;
                
                if(size < kMinimumSize)
                    size = kMinimumSize;
            }
            
            if(size == curTableSize)
                return 0;
            
            return size * sizeof(ValueType*);
        }
        
        bool Resize(void* allocation, size_t size, bool force = false, void** oldTable = NULL)
        {
            if(!force && size != ResizeNeeded())
            {
                allocator.Free(allocation);
                return false;
            }
            
            _Resize((ValueType**)allocation, size / sizeof(ValueType*), oldTable);
            return true;
        }
        
    protected:
        
        /* Appends value as head to list of value and saves this new head in the storage */
        void _Insert(ValueType** table, size_t tableSize, ValueType* value)
        {
            size_t index = definition.Hash(value) & (tableSize - 1);
            
            _Link(value) = table[index];
            table[index] = value;
        }
        
        /* Returns the head of the list of the item */
        ValueType*& _Link(ValueType* bucket) const
        {
            return definition.GetLink(bucket);
        }
        
        bool _Resize(size_t newSize)
        {
            ValueType** newStorage = (ValueType**)allocator.Allocate(sizeof(ValueType*) * newSize);
            if(!newStorage)
                return false;
            
            _Resize(newStorage, newSize);
            return true;
        }
        
        void _Resize(ValueType** newStorage, size_t newSize, void** oldStorage = NULL)
        {
            for(size_t i = 0; i < newSize; i++)
                newStorage[i] = NULL;
            
            if(storage)
            {
                for(size_t i = 0; i < curTableSize; i++)
                {
                    ValueType* bucket = storage[i];
                    while(bucket)
                    {
                        ValueType* next = _Link(bucket);
                        _Insert(newStorage, newSize, bucket);
                        bucket = next;
                    }
                }
                
                if(oldStorage)
                    *oldStorage = storage;
                else
                    allocator.Free(storage);
            }
            
            curTableSize = newSize;
            storage = newStorage;
        }
        
        bool _ExhaustiveSearch(ValueType* value) const
        {
            for(size_t i = 0; i < curTableSize; i++)
            {
                ValueType* bucket = storage[i];
                while(bucket)
                {
                    if(bucket == value)
                        return true;
                    
                    bucket = _Link(bucket);
                }
            }
            
            return false;
        }
        
        Definition      definition;
        Allocator       allocator;
        size_t          curTableSize;
        size_t          curItemCount;
        ValueType**     storage;
    };
}

#endif
