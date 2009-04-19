#ifndef MEMORY_MANAGER_H_
#define MEMORY_MANAGER_H_

#include <lib/types.h>

namespace Memory
{
    class MemoryManager
    {
    public:
        MemoryManager();
        bool HeapInitialized();
        
        void* kmalloc(size_t size, bool pageAlign, Address* physicalAddress = NULL);
        void* kcalloc(unsigned int n, size_t s, bool pageAlign, Address* physicalAddress = NULL);
        void free(void* p);

        //void InitializeHeap(); //TODO: implement this
        
    private:
        static MemoryManager* instance;

        unsigned int placement_address;
        bool heapInitialized;
        
        //Heap kernelHeap //TODO: implement this
    };
}
#endif
