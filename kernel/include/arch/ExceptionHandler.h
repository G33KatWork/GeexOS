#ifndef EXCEPTIONHANDLER_H_
#define EXCEPTIONHANDLER_H_

#include <kernel/debug.h>
#include <lib/types.h>

class InvalidOpcodeHandler : public IInterruptServiceRoutine
{
public:
    void Execute(registers_t *regs)
    {
        MAIN_DEBUG_MSG("Invalid Opcode: EIP: " << IO::hex << (unsigned) regs->eip);
        PANIC("Invalid opcode!");
        for(;;);
    }
};

class ExceptionHandler : public IInterruptServiceRoutine
{
public:
    void Execute(registers_t *regs)
    {
        MAIN_DEBUG_MSG("Exception " << IO::dec << regs->int_no << ": EIP: " << IO::hex << (unsigned) regs->eip << ": Errorcode: " << IO::dec << (unsigned)regs->err_code);
        PANIC("Unhandled exception!");
        for(;;);
    }
};
    
#endif
