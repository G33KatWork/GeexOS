#ifndef STACK_H_
#define STACK_H_

#include <lib/types.h>

namespace Memory
{
    class Stack
    {
    public:
        Stack(Address end, size_t s);
        
        Address GetStartAddress();
        unsigned int GetSize();
        Address GetEndAddress();
        void MoveCurrentStackHere(Address oldESP);
        
    private:
        Address endAddr;
        size_t size;
    };
}
#endif
