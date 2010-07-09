#include <arch/x86InterruptDispatcher.h>
#include <arch/internal/PIC.h>
#include <arch/HAL.h>
#include <kernel/global.h>
#include <kernel/debug.h>

using namespace Arch;
using namespace Debug;

int nestedExceptions = 0;

x86InterruptDispatcher::x86InterruptDispatcher()
{
    for(int i = 0; i < I86_IDT_MAX_ENTRY_COUNT; i++) isrs[i] = NULL;
}

bool x86InterruptDispatcher::RegisterInterruptHandler(InterruptingDevice i, InterruptServiceRoutine* isr)
{
    int intno = interruptToNumber(i);
    if(intno == -1)
        return false;
    
    isrs[intno] = isr;
    return true;
}

bool x86InterruptDispatcher::UnregisterInterruptHandler(InterruptingDevice i)
{
    int intno = interruptToNumber(i);
    if(intno == -1)
        return false;
    
    isrs[intno] = NULL;
    return true;
}

bool x86InterruptDispatcher::RegisterExceptionHandler(Exception e, InterruptServiceRoutine* isr)
{
    int intno = exceptionToNumber(e);
    if(intno == -1)
        return false;
    
    isrs[intno] = isr;
    return true;
}

bool x86InterruptDispatcher::UnregisterExceptionHandler(Exception e)
{
    int intno = exceptionToNumber(e);
    if(intno == -1)
        return false;
    
    isrs[intno] = NULL;
    return true;
}

void x86InterruptDispatcher::MaskInterrupt(InterruptingDevice i)
{
    int intno = interruptToNumber(i);
    if(intno == -1)
        return;
    
    MaskIRQ(intno);
}

void x86InterruptDispatcher::UnmaskInterrupt(InterruptingDevice i)
{
    int intno = interruptToNumber(i);
    if(intno == -1)
        return;
    
    UnmaskIRQ(intno);
}

void x86InterruptDispatcher::Execute(registers_t *regs)
{
    NotifyPIC(regs->int_no);

    /*ARCH_INTERRUPTS_DEBUG_MSG("Executing interrupt");
    ARCH_INTERRUPTS_DEBUG_MSG("Register state:");
    ARCH_INTERRUPTS_DEBUG_MSG("EIP: " << hex << regs->eip);
    ARCH_INTERRUPTS_DEBUG_MSG("CS: " << regs->cs);
    ARCH_INTERRUPTS_DEBUG_MSG("EFLAGS: " << regs->eflags);
    ARCH_INTERRUPTS_DEBUG_MSG("USERESP: " << regs->useresp);
    ARCH_INTERRUPTS_DEBUG_MSG("SS: " << regs->ss);
    ARCH_INTERRUPTS_DEBUG_MSG("INTNO: " << regs->int_no);
    ARCH_INTERRUPTS_DEBUG_MSG("ERRCODE: " << regs->err_code);
    ARCH_INTERRUPTS_DEBUG_MSG("EAX: " << regs->eax);
    ARCH_INTERRUPTS_DEBUG_MSG("ECX: " << regs->ecx);
    ARCH_INTERRUPTS_DEBUG_MSG("EDX: " << regs->edx);
    ARCH_INTERRUPTS_DEBUG_MSG("EBX: " << regs->ebx);
    ARCH_INTERRUPTS_DEBUG_MSG("ESP: " << regs->esp);
    ARCH_INTERRUPTS_DEBUG_MSG("EBP: " << regs->ebp);
    ARCH_INTERRUPTS_DEBUG_MSG("ESI: " << regs->esi);
    ARCH_INTERRUPTS_DEBUG_MSG("EDI: " << regs->edi);
    ARCH_INTERRUPTS_DEBUG_MSG("DS: " << regs->ds);*/
    
    if(isrs[regs->int_no] != NULL)
    {
        InterruptServiceRoutine* isr = isrs[regs->int_no];
        isr->Execute(regs);
    }
    else
    {
        if(regs->int_no < 32)
        {
            ARCH_INTERRUPTS_DEBUG_MSG("Unhandled fault: " << dec << regs->int_no);
        }
        else
        {
            ARCH_INTERRUPTS_DEBUG_MSG("Unhandled interrupt: " << dec << regs->int_no);
        }
    }
}

int x86InterruptDispatcher::exceptionToNumber(Exception e)
{
    switch(e)
    {
        case EXC_DIVBY0:
            return 0;
        case EXC_DEBUG:
            return 1;
        case EXC_NMI:
            return 2;
        case EXC_BREAKPOINT:
            return 3;
        case EXC_OVERFLOW:
            return 4;
        case EXC_BOUNDCHECK:
            return 5;
        case EXC_INVALIDOPCODE:
            return 6;
        case EXC_COPROCNOTAVAIL:
            return 7;
        case EXC_DOUBLEFAULT:
            return 8;
        case EXC_COPROCSEGMENTOVERRUN:
            return 9;
        case EXC_INVALIDTSS:
            return 10;
        case EXC_SEGMENTNOTPRESENT:
            return 11;
        case EXC_STACK:
            return 12;
        case EXC_GENERALPROTECTIONFAULT:
            return 13;
        case EXC_PAGEFAULT:
            return 14;
        case EXC_COPROCERROR:
            return 16;
        default:
            return -1;
    }
}

int x86InterruptDispatcher::interruptToNumber(InterruptingDevice i)
{
    switch(i)
    {
        case IRDEV_TIMER:
            return 32 + 0;
        case IRDEV_KEYBOARD:
            return 32 + 1;
        case IRDEV_SERIAL1:
            return 32 + 4;
        case IRDEV_SERIAL2:
            return 32 + 3;
        case IRDEV_PARALLEL2:
            return 32 + 5;
        case IRDEV_FLOPPY:
            return 32 + 6;
        case IRDEV_PARALLEL1:
            return 32 + 7;
        case IRDEV_RTC:
            return 32 + 8;
        case IRDEV_PS2MOUSE:
            return 32 + 12;
        case IRDEV_FPU:
            return 32 + 13;
        case IRDEV_ATAPRIMARY:
            return 32 + 14;
        case IRDEV_ATASLAVE:
            return 32 + 15;
        default:
            return -1;
    }
}

void fault_handler(registers_t regs)
{
    DEBUG_MSG("ex: " << regs.int_no);
    if(nestedExceptions > MAX_NESTED_EXCEPTIONS) PANIC("Maximum number of nested exceptions reached.");
    nestedExceptions++;
    
    CurrentHAL->GetInterruptDispatcher()->Execute(&regs);
    
    nestedExceptions--;
}

void interrupt_handler(registers_t regs)
{
    CurrentHAL->GetInterruptDispatcher()->Execute(&regs);
}

