#ifndef STACK_H_
#define STACK_H_

#include <lib/types.h>

namespace Memory
{
    class Stack
    {
    public:
        Stack(Address end, size_t s);
        
        Address GetStartAddress() { return endAddr + size; }
        unsigned int GetSize() { return size; }
        Address GetEndAddress() { return endAddr; }
        
        void MoveCurrentStackHere(Address oldESP);
        void PrintStacktrace(unsigned int n = 15);
        
    private:
        Address endAddr;
        size_t size;
    };
}
#endif
