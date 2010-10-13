#ifndef HEAP_H_
#define HEAP_H_

#include <types.h>
#include <kernel/Memory/IMemoryAllocator.h>
#include <kernel/DataStructures/OrderedArray.h>

using namespace DataStructures;

namespace Memory
{
    #define     HEAP_INDEX_SIZE  	0x20000         //Increment this, if you need more items to be saved on the heap
    #define     HEAP_MAGIC       	0x123890AB      //Some magic value to find headers and footers
    #define     HEAP_MIN_SIZE   	0x70000         //Minimum size the heap will shrink to
    
    /**
       Size information for a hole/block
     **/
    typedef struct
    {
    	uint32_t magic;     // Magic number, used for error checking and identification.
    	bool isHole;        // true if this is a hole. false if this is a block.
    	uint32_t size;      // size of the block, including the end footer.
    } header_t;

    typedef struct
    {
    	uint32_t magic;     // Magic number, same as in header_t.
    	header_t *header;   // Pointer to the block header.
    } footer_t;
    
    class Heap : public IMemoryAllocator
    {
    private:
        Address startAddress;
        Address endAddress;
        Address maxAddress;
        
        OrderedArray<header_t*, HEAP_INDEX_SIZE> *index;
        
        void expand(size_t newSize);
        int findSmallestHole(size_t size, bool pageAlign);
            
	public:
        Heap(Address location, size_t size, size_t initialSize);
        
        void* Allocate(size_t len, bool pageAlign);
        void Deallocate(void* p);
        
    };
}
#endif
