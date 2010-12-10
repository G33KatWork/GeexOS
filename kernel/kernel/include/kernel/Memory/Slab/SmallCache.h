#ifndef _SLAB_ALLOCATOR_SMALLCACHE_H_
#define _SLAB_ALLOCATOR_SMALLCACHE_H_

#include <arch/types.h>
#include <types.h>
#include <kernel/Memory/Slab/SlabCache.h>
#include <arch/HAL.h>

/*  Layout of a Slab in a SmallCache:
 *  ______________________________________________________________________________
 * |                |                             |         |                     |
 * | Slab structure |         Free Array          | Wastage |      Objects        |
 * |  sizeof(Slab)  | objCount * sizeof(uint32_t) |         | objCount * objSize  |
 * |________________|_____________________________|_________|_____________________|
 * 
 *                  ^                                       ^
 *                  |                                       |
 *            GetFreeArray(slab)                    slab->objectStart
 */

namespace Memory
{
    namespace Slab
    {
        class SlabAllocator;
        
        class SmallCache : public SlabCache
        {
        private:
            //the free-array is directly behind the Slab class itself, so increment the pointer and cast to array pointer
            inline uint32_t* GetFreeArray(Slab* slab) { return (uint32_t*)(slab + 1); }
        
        protected:
            virtual Slab* Grow();
        
        public:
            SmallCache(const char* Name, int Align, int Size, SlabAllocator* ParentAllocator)
                : SlabCache(Name, Align, Size, ParentAllocator)
            { }
        
            virtual void* AllocateObject();
            virtual void FreeObject(void* object);
            
            virtual void ReleaseSlab(Slab* slab);
        };
    }
}
#endif
