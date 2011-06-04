#include <physicalMemory.h>

/*#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

uint32_t numFrames;
uint32_t *frames;

static inline void setFrame(Address a)
{
    uint32_t frame = a / 0x1000;
    uint32_t index = INDEX_FROM_BIT(frame);
    uint32_t offset = OFFSET_FROM_BIT(frame);
    frames[index] |= (1 << offset);
}

static inline void clearFrame(Address a)
{
    uint32_t frame = a / 0x1000;
    uint32_t index = INDEX_FROM_BIT(frame);
    uint32_t offset = OFFSET_FROM_BIT(frame);
    frames[index] &= ~(1 << offset);
}

static inline uint32_t testFrame(Address a)
{
    uint32_t frame = a / 0x1000;
    uint32_t index = INDEX_FROM_BIT(frame);
    uint32_t offset = OFFSET_FROM_BIT(frame);
    return frames[index] & (1 << offset);
}

void physmem_init(uint32_t memSize)
{
    
}

Address physmem_allocateFrame(void)
{
    
}
*/