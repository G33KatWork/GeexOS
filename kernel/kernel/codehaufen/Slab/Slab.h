#ifndef SLAB_H_
#define SLAB_H_

#include <types.h>
#include <kernel/global.h>
#include <arch/paging.h>

#define __ALIGN_MASK(x,mask)    (((x)+(mask))&~(mask))
#define ALIGN(x,a)      __ALIGN_MASK(x,(typeof(x))(a)-1)

#define DEBUG_LSLAB(x)  DEBUG_MSG(x)

namespace Memory
{
    namespace Slab
    {
        using namespace IO;
        
        struct bufctl_ {
            Address bufferBeginning;
            struct bufctl_* next;
        };
        typedef struct bufctl_ bufctl;
        
        class LargeSlab
        {
        public:
            LargeSlab(size_t size, size_t align)
            {
                objectSize = size;
                objectAlign = align;
                
                DEBUG_LSLAB("Creating large slab with size " << size << " and align " << align);
                
               if(align)
                    size = ALIGN(size, align);
                DEBUG_LSLAB("Aligned size: " << (unsigned)size);
                
                unsigned int gfporder;
                size_t remainder;
                
                for(gfporder = 0; gfporder <= 25; gfporder++)
                {
                    DEBUG_LSLAB("gfporder " << (unsigned)gfporder);
                    slabSize = PAGE_SIZE << gfporder;
                    numberOfObjects = slabSize / size;
                    remainder = slabSize - numberOfObjects * size;
                    DEBUG_LSLAB("Total slabsize would be " << slabSize << " Number of objects " << numberOfObjects << " with remainder of " << (unsigned)remainder);
                    
                    if(!numberOfObjects)
                        continue;
                    
                    //fragmentation is acceptable if it's not worth taking another page
                    if(remainder * 8 <= (unsigned)(PAGE_SIZE << gfporder))
                        break;
                }
            }
            
            ~LargeSlab(){}
            
        private:
            size_t slabSize;
            unsigned int numberOfObjects;
            
            size_t objectSize;
            size_t objectAlign;
        };
        
        class SmallSlab
        {
        public:
            SmallSlab(size_t size, int align)
            {
                bufsize = size;
                bufAlign = align;
                physicalBufferPage = memoryManager.AllocateFrame();
            }
            
            ~SmallSlab(){}
        
        private:
            Address physicalBufferPage;
            
            size_t bufsize;
            int bufAlign;
        };
    }
}
#endif
