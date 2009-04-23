#ifndef HEAP_H_
#define HEAP_H_

#include <lib/types.h>

namespace Memory
{
    class Heap
    {
    public:
        Heap();
        
        void* AllocateMemory(size_t s);
        void FreeMemory(void* p);
        
    private:
    };
}
#endif
