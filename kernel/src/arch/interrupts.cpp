#include <arch/interrupts.h>
#include <kernel/global.h>
#include <kernel/IO/Monitor.h>
#include <arch/pit.h>
#include <arch/pic.h>

using namespace Arch;
using namespace Kernel;
using namespace IO;

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
    InitializePIC();
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

