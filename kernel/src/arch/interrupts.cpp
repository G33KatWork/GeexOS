#include <arch/interrupts.h>
#include <kernel/global.h>
#include <arch/pit.h>
#include <arch/pic.h>
#include <kernel/debug.h>

using namespace IO;
using namespace Arch;
using namespace Kernel;

int nestedExceptions = 0;

void fault_handler(registers_t regs)
{
    if(nestedExceptions > MAX_NESTED_EXCEPTIONS) PANIC("Maximum number of nested exceptions reached.");
    nestedExceptions++;
    
    InterruptDispatcher::GetInstance()->Execute(&regs);
    
    nestedExceptions--;
}

void interrupt_handler(registers_t regs)
{
    InterruptDispatcher::GetInstance()->Execute(&regs);
}

InterruptDispatcher* InterruptDispatcher::instance = NULL;

InterruptDispatcher* InterruptDispatcher::GetInstance()
{
    if(instance == NULL)
        instance = new InterruptDispatcher();
    
    return instance;
}


InterruptDispatcher::InterruptDispatcher()
{
    for(int i = 0; i < I86_IDT_MAX_ENTRY_COUNT; i++) isrs[i] = NULL;
}


void InterruptDispatcher::RegisterHandler(int i, IInterruptServiceRoutine* isr)
{
    ASSERT(i >= 0 && i <= I86_IDT_MAX_ENTRY_COUNT, "Interrupt number out of valid range");
    isrs[i] = isr;
}

void InterruptDispatcher::UnregisterHandler(int i)
{
    ASSERT(i >= 0 && i <= I86_IDT_MAX_ENTRY_COUNT, "Interrupt number out of valid range");
    isrs[i] = NULL;
}

void InterruptDispatcher::Execute(registers_t *regs)
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
        IInterruptServiceRoutine* isr = isrs[regs->int_no];
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

