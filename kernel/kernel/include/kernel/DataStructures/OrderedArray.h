#ifndef DATASTRUCTURES_ORDEREDARRAY_H_
#define DATASTRUCTURES_ORDEREDARRAY_H_

#include <types.h>
#include <kernel/global.h>

namespace DataStructures
{
    template <class T, unsigned int capacity>
    class OrderedArray
    {
    public:
        OrderedArray()
        {
            for(unsigned int i = 0; i < capacity; i++)
                arr[i] = NULL;
            
            size = 0;
            lessThan = (bool (*)(T, T)) NULL;
        }
        
        void SetPredicate(bool (*pred)(T, T))
        {
            lessThan = pred;
        }
        
        void Insert(T item)
        {
            ASSERT(lessThan, "lessThan predicate must be set");
            ASSERT(size + 1 < capacity, "Array is full");
            unsigned int i = 0;
            
            //search insertion position
            while(i < size && lessThan(arr[i], item))
                i++;
            
            if(i == size) //reached end
                arr[size++] = item;
            else
            {
                T tmp = arr[i];
                arr[i] = item;
                while(i < size)
                {
                    i++;
                    T tmp2 = arr[i];
                    arr[i] = tmp;
                    tmp = tmp2;
                }
                
                size++;
            }
        }
        
        T ItemAt(unsigned int index)
        {
            return arr[index];
        }
        
        void RemoveAt(unsigned int index)
        {
            while(index < size)
            {
                arr[index] = arr[index + 1];
                index++;
            }
            
            size--;
        }
        
        void DebugDump()
        {
            /*kdbg << "Dumping ordered array at " << hex << (unsigned)this << endl;
            
            for(int i = 0; i < size; i++)
                kdbg << "\t" << (unsigned)arr[i] << endl;
            
            kdbg << endl;*/
        }
        
        unsigned int GetSize()
        {
            return size;
        }
    
    private:
        T arr[capacity];
        unsigned int size;
        bool (*lessThan)(T, T);
    };
}

#endif
