#ifndef DATASTRUCTURES_BITFIELD_H_
#define DATASTRUCTURES_BITFIELD_H_

#include <types.h>

namespace DataStructures
{
    class Bitfield
    {
    private:
        uint32_t numElements;
        uint32_t *field;
 
    public:
        Bitfield(uint32_t NumElements);
        ~Bitfield();
        
        void SetBit(uint32_t Index);
        void ClearBit(uint32_t Index);
        bool IsBitSet(uint32_t Index);
        uint32_t GetFirstClearedIndex();
        
        uint32_t GetNumElements() { return numElements; }
    };
}

#endif
