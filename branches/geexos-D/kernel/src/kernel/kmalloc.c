#include <kernel/kmalloc.h>

extern uint32_t end;
static uint32_t placement_address = (uint32_t)&end;

void *kmalloc(size_t s)
{
    uint32_t tmp;
    tmp = placement_address;
    placement_address += s;
    return (void*)tmp;
}
