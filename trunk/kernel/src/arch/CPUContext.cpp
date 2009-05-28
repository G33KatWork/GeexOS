#include <arch/CPUContext.h>
#include <lib/types.h>
#include <kernel/global.h>

using namespace Arch;

#define STACKSIZE 0x4000

static bool context_initialized = false;
extern Address bootStack;

using namespace IO;

CPUContext::CPUContext(void (*func)(void *), void* arg)
{
    if(!context_initialized)
    {
        context.kernelStack = &bootStack;
        context_initialized = true;
        return;
    }
    
    context.kernelStack = (Address*)kmalloc(STACKSIZE);
    
    unsigned int *stack = (unsigned int*)(context.kernelStack + STACKSIZE);
    
    //Push argument for function
    *--stack = (unsigned int)arg;
    
    //space for context switch to write return address
    *--stack = (unsigned int)0;
    *--stack = (unsigned int)0;
    
    context.ip = (Address)func;
    context.sp = (Address)stack;
    context.bp = 0;
    context.bx = 0;
    context.edi = 0;
    context.esi = 0;
}

CPUContext::~CPUContext()
{
    kfree(context.kernelStack);
}

void CPUContext::SwitchTo(CPUContext *old)
{
    //TODO: Set kernel stack in TSS
    
    /*kout << Monitor::SetPosition(10, 12);
    kout << "context will be restored: " << endl;
    kout << "IP: " << hex << context.ip << endl;
    kout << "SP: " << hex << context.sp << endl;*/
    
    
    if(context_save(old->GetContext()) == 0) {
        /*kout << Monitor::SetPosition(10, 16);
        kout << "context saved: " << endl;
        kout << "IP: " << hex << old->GetContext()->ip << endl;
        kout << "SP: " << hex << old->GetContext()->sp << endl;*/
        context_restore(&context);
    }
}
