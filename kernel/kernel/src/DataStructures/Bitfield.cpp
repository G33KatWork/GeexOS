#include <kernel/DataStructures/Bitfield.h>
#include <string.h>
#include <kernel/global.h>

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

using namespace DataStructures;
using namespace Debug;

Bitfield::Bitfield(uint32_t NumElements)
{
    numElements = NumElements;
	field = (uint32_t*)kmalloc((numElements / sizeof(uint32_t)) + sizeof(uint32_t));
	memset(field, 0, (numElements / sizeof(uint32_t)) + sizeof(uint32_t));
	
	BITFIELD_DEBUG_MSG("Allocated bitfield with " << dec << numElements <<
	                   " elements. Resulting size: " << dec << 
	                   (numElements / sizeof(uint32_t)) + sizeof(uint32_t) << " Bytes");
}

Bitfield::~Bitfield()
{
    kfree(field);
}

void Bitfield::SetBit(uint32_t Index)
{
    BITFIELD_DEBUG_MSG("Setting bit " << dec << Index);
    ASSERT(Index < numElements, "Argument of bounds");
    if(Index >= numElements)
        return;
    
    unsigned int idx = INDEX_FROM_BIT(Index);
	unsigned int off = OFFSET_FROM_BIT(Index);
	field[idx] |= (0x1 << off);
}

void Bitfield::ClearBit(uint32_t Index)
{
    BITFIELD_DEBUG_MSG("Clearing bit " << dec << Index);
    ASSERT(Index < numElements, "Argument of bounds");
    if(Index >= numElements)
        return;
    
    unsigned int idx = INDEX_FROM_BIT(Index);
	unsigned int off = OFFSET_FROM_BIT(Index);
	field[idx] &= ~(0x1 << off);
}

bool Bitfield::IsBitSet(uint32_t Index)
{
    BITFIELD_DEBUG_MSG("Checking if bit " << dec << Index << " is set");
    ASSERT(Index < numElements, "Argument of bounds");
    if(Index >= numElements)
        return false;
    
    unsigned int idx = INDEX_FROM_BIT(Index);
	unsigned int off = OFFSET_FROM_BIT(Index);
	return field[idx] & (0x1 << off);
}

uint32_t Bitfield::GetFirstClearedIndex()
{
    for(unsigned int i = 0; i < INDEX_FROM_BIT(numElements); i++)
	{
		if(field[i] != 0xFFFFFFFF)	//nothing is free here
		{
			// at least one bit is free here
			for(unsigned int j = 0; j < 32; j++)
			{
				unsigned int toTest = 0x1 << j;
				if( !(field[i]&toTest) )
				{
				    BITFIELD_DEBUG_MSG("Returning bit " << dec << (i*4*8+j) << " as first cleared bit");
    				return (i*4*8+j);	//32 bit (4*8) per entry;
    			}
			}
		}
	}
	
	//error
	BITFIELD_DEBUG_MSG("ERROR: No cleared bits found");
	return (unsigned int)-1;
}
