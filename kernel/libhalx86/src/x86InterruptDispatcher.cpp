#include <arch/x86InterruptDispatcher.h>
#include <arch/HAL.h>
#include <kernel/global.h>
#include <kernel/debug.h>
#include <arch/internal/x86ResolveableException.h>

using namespace Arch;
using namespace Debug;

int nestedExceptions = 0;

x86InterruptDispatcher::x86InterruptDispatcher()
{
    for(int i = 0; i < I86_IDT_MAX_ENTRY_COUNT; i++) isrs[i] = NULL;
}

bool x86InterruptDispatcher::RegisterInterruptVector(uint32_t vector, InterruptServiceRoutine* isr)
{    
    ASSERT(vector < I86_IDT_MAX_ENTRY_COUNT, "Index out of bounds");

    isrs[vector] = isr;
    return true;
}

bool x86InterruptDispatcher::UnregisterInterruptVector(uint32_t vector)
{
    ASSERT(vector < I86_IDT_MAX_ENTRY_COUNT, "Index out of bounds");
    
    isrs[vector] = NULL;
    return true;
}

void x86InterruptDispatcher::Execute(registers_t* regs, bool isException)
{
    if(isrs[regs->int_no] != NULL)
    {
        InterruptServiceRoutine* isr = isrs[regs->int_no];
        if(isException)
            ((x86ResolveableException*)isr)->Prepare(regs);
        isr->Execute();
    }
    else
    {
        ARCH_INTERRUPTS_DEBUG_MSG("Unhandled io IRQ: " << dec << regs->int_no);
    }
}

void fault_handler(registers_t regs)
{
    //DEBUG_MSG("ex: " << regs.int_no);
    if(nestedExceptions > MAX_NESTED_EXCEPTIONS) PANIC("Maximum number of nested exceptions reached.");
    nestedExceptions++;
    
    //Casting should be safe here
    ((x86InterruptDispatcher*)CurrentHAL->GetInterruptDispatcher())->Execute(&regs, true);
    
    nestedExceptions--;
}

void interrupt_handler(registers_t regs)
{
    ((x86InterruptDispatcher*)CurrentHAL->GetInterruptDispatcher())->Execute(&regs, false);
}

