#include <arch/PageFaultHandler.h>
#include <kernel/global.h>
#include <kernel/debug.h>

using namespace IO;
using namespace Arch;

void PageFaultHandler::Execute(registers_t *regs)
{
    //A page fault has occurred.
    //The faulting address is stored in the CR2 register.
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
    
    //The error code gives us details of what happened.
    int present   = !(regs->err_code & 0x1); // Page not present
    int rw = regs->err_code & 0x2;           // Write operation?
    int us = regs->err_code & 0x4;           // Processor was in user-mode?
    int reserved = regs->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    int id = regs->err_code & 0x10;          // Caused by an instruction fetch?

    kdbg.SetForeground(Red);
    kdbg << "Page fault! ( " << (present ? "present " : "") << (rw ? "read-only " : "")
        << (us ? "user-mode " : "") << (reserved ? "reserved " : "")
        << (id ? "instruction-fetch " : "") << ") at " << hex << faulting_address << " EIP: " << regs->eip << endl;
        
    PANIC("Page fault!");
}
