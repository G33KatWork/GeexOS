#ifndef _ARCH_CPU_CONTEXT_H
#define _ARCH_CPU_CONTEXT_H

#include <lib/types.h>

namespace Arch
{
    typedef struct {
        unsigned short GS;
        unsigned short FS;
        unsigned short DS;
        unsigned short SS;
        unsigned short CS;
        unsigned short ES;
        
        Address ip;         //EIP (Instruction Pointer)
        Address sp;         //ESP (Stack Pointer)
        unsigned int bp;    //EBP (Stack Frame)
        unsigned int bx;    //EBX
        unsigned int edi;   //EDI
        unsigned int esi;   //ESI
        unsigned int edx;   //EDX
        unsigned int ecx;   //ECX
        unsigned int eax;   //EAX
        unsigned int eflags; //EFLAGS
        
        Address *kernelStack;   //KernelStack
    } context_t;
    
    extern "C" int context_save(context_t *ctx);
    extern "C" void context_restore(context_t *ctx);
    
    class CPUContext
    {
    private:
        context_t context;
        
    public:
        CPUContext(void (*func)(void *), void* arg);
        ~CPUContext();
        
        context_t* GetContext() { return &context; }
        
        void SwitchTo(CPUContext *old);
    };
}

#endif
