#ifndef _HAL_X86_RESOLVEABLEEXCEPTION_H
#define _HAL_X86_RESOLVEABLEEXCEPTION_H

#include <halinterface/InterruptServiceRoutine.h>
#include <kernel/global.h>
#include <arch/internal/registers.h>

namespace Arch
{
    class x86ResolveableException : public InterruptServiceRoutine
    {
    protected:
        virtual bool Resolve() = 0;
        
    public:
        virtual void Prepare(registers_t* regs) = 0;
        
        virtual void Execute()
        {
            if(!Resolve())
                PANIC("Unrecoverable exception occured");
        }
    };
}

#endif
