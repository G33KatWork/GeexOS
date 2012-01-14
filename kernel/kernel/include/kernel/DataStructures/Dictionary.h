#ifndef _DATASTRUCTURES_DICTIONARY_H_
#define _DATASTRUCTURES_DICTIONARY_H_

#include <kernel/DataStructures/Hashtable.h>
#include <kernel/Utils/CRC32.h>

//TODO: iterators

namespace DataStructures {

//KeyValuePair as a container for the dictionary
template <typename DictionaryKeyType, typename DictionaryValueType>
class KeyValuePair
{
    template<typename, typename, typename, typename>
    friend class Dictionary;

public:
    DictionaryKeyType   key;
    DictionaryValueType value;

    KeyValuePair(DictionaryKeyType k, DictionaryValueType v)
        : key(k),
        value(v)
    {}

private:
    struct HashTableDefinition
    {
        typedef DictionaryKeyType   KeyType;
        typedef KeyValuePair        ValueType;

        inline size_t HashKey(const KeyType key) const { return (size_t)CRC32((char*)&key, sizeof(KeyType)); } //FIXME: Breaks for pointer key types
        inline size_t Hash(ValueType* value) const { return HashKey(value->key); }
        inline bool Compare(KeyType key, ValueType* value) const { return value->key == key; }
        inline ValueType*& GetLink(ValueType* value) const { return value->next; }
    };

    KeyValuePair<DictionaryKeyType, DictionaryValueType>* next;
};

//The dictionary implemented with a HashTable and some wrapperfunctions for allocating and deallocating the KeyValuePairs
//TODO: Let user choose KeyValuePair allocator with template stuff
template <typename KeyType, typename ValueType, typename Hasher, typename Allocator = KMallocAllocator>
class Dictionary
{
protected:
    typedef     struct KeyValuePair<KeyType, ValueType>         KeyValuePairType;
    typedef     struct KeyValuePairType::HashTableDefinition    DictHashTableDefinition;
    
    BHashTable<DictHashTableDefinition, true, false, Allocator> hashTable;

public:
    size_t GetItemCount() const { return hashTable.GetItemCount(); }
    ValueType& Lookup(const KeyType& key) const { return hashTable.Lookup(key)->value; }
    
    ~Dictionary() { Clear(); }
    
    void Clear()
    {
        KeyValuePair<KeyType, ValueType>* itemList = hashTable.Clear(true);
        while(itemList)
        {
            KeyValuePair<KeyType, ValueType>* next = itemList->next;
            delete itemList;
            itemList = next;
        }
    }

    bool Insert(const KeyType& key, ValueType value)
    { 
        KeyValuePair<KeyType, ValueType> *kv = new KeyValuePair<KeyType, ValueType>(key, value);
        return hashTable.Insert(kv);
    }

    bool Remove(const KeyType& key)
    {
        KeyValuePair<KeyType, ValueType>* toRemove = hashTable.Lookup(key);
        if(toRemove)
        {
            bool ret = hashTable.Remove(toRemove);
            delete toRemove;
            return ret;
        }
        else
            return false;
    }

    ValueType& operator[] (const KeyType& key) const { return this->Lookup(key); }
};

}

#endif
