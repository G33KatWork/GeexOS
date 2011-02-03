/*#ifndef DATASTRUCTURES_VECTOR_H_
#define DATASTRUCTURES_VECTOR_H_

#include <types.h>
#include <arch/types.h>
#include <string.h>

namespace DataStructures
{
    template<typename T>
    class Vector
    {
    private:
        static const size_t kDefaultChunkSize = 10;
        static const size_t kMaximalChunkSize = 1024 * 1024;
        
        size_t currentCapacity;
        size_t chunkSize;
        unsigned int itemCount;
        T *items;
        
    public:
        Vector(size_t chunkSize_ = kDefaultChunkSize)
        : currentCapacity(0),
          chunkSize(chunkSize_),
          itemCount(0),
          items(NULL)
        {
            if(chunkSize == 0 || chunkSize > kMaximalChunkSize)
                chunkSize = kDefaultChunkSize;
            
            Resize(0);
        }
        
        ~Vector()
        {
            Clear();
            kfree(items);
        }
        
        void PushFront(const T& val) { Insert(val, 0); }
        void PushBack(const T& val) { Insert(val, itemCount); }
        
        void PopFront() { if(itemCount > 0) RemoveAt(0); }
        void PopBack() { if(itemCount > 0) RemoveAt(itemCount - 1); }
        
        inline unsigned int Count() const { return itemCount; }
        inline bool IsEmpty() const { return (itemCount == 0); }
        
        inline T &operator[](unsigned int index) { return ElementAt(index); }
        
        bool Insert(const T& val, unsigned int index)
        {
            if(index > itemCount)
                return false;
            
            if(!Resize(itemCount + 1))
                return false;
            
            MoveItems(items + index, 1, itemCount - index - 1);
            new(items + index) T(val);  //call copy constructor
            return true;
        }
        
        void RemoveAt(unsigned int index)
        {
            if(index < itemCount)
            {
                items[index].~T();
                MoveItems(items + index + 1, -1, itemCount - index - 1);
                Resize(itemCount - 1);
            }
        }
        
        void Remove(const T& val)
        {
            for(unsigned int i = 0; i < itemCount; i++)
            {
                if(ElementAt == val)
                {
                    RemoveAt(i);
                }
            }
        }
        
        void Clear()
        {
            for(unsigned int i = 0; i < itemCount; i++)
            {
                items[i].~T();
                RemoveAt(i);
            }
        }
        
        inline const T &ElementAt(unsigned int index) const
        {
            if(index >= 0 && index <= itemCount)
                return items[index];
            
            return NULL;
        }
        
    private:
        bool Resize(size_t count)
        {
            bool result = true;
            size_t newSize = count;
            
            if(newSize <= 0)
                newSize = 1;
            
            newSize = ((newSize - 1) / chunkSize + 1) * chunkSize;
            if(newSize != currentCapacity)
            {
                T* newItems = (T*)krealloc(items, newSize * sizeof(T));
                if(newItems)
                {
                    items = newItems;
                    currentCapacity = newSize;
                }
                else
                    result = false;
            }
            
            if(result)
                itemCount = count;
            
            return result;
        }
        
        inline static void MoveItems(T *values, unsigned int offset, unsigned int count)
        {
            if(count > 0 && offset != 0)
                memmove(values + offset, values, count * sizeof(T));
        }
    };
}

#endif
*/