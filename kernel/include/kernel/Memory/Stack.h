#ifndef STACK_H_
#define STACK_H_

#include <lib/types.h>

namespace Memory
{
    class Stack
    {
    public:
        Stack(Address addr, unsigned int s);
        Stack(unsigned int s);
        
        Address GetAddress();
        unsigned int GetSize();
        Address GetEndAddress();
        
    private:
        Address address;
        unsigned int size;
    };
}
#endif
