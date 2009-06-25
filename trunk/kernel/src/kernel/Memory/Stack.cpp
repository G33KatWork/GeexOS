#include <kernel/Memory/Stack.h>
#include <kernel/global.h>

using namespace Memory;

Stack::Stack(Address addr, unsigned int s)
{
    this->address = addr;
    this->size = s;
}

Stack::Stack(unsigned int s)
{
    PANIC("Stack allocation not yet implemented");
}

Address Stack::GetAddress()
{
    return address;
}

unsigned int Stack::GetSize()
{
    return size;
}

Address Stack::GetEndAddress()
{
    return address + size;
}
