#include <arch/x86ThreadContext.h>
#include <string.h>
#include <arch/internal/GDT.h>
#include <arch/HAL.h>

using namespace Arch;

void x86ThreadContext::initializeForKernelmode(Address InitialIP, Address InitialSP, Address InitialFP, BasePageDirectory* PageDirectory)
{
    memset(&threadInfo, 0, sizeof(x86ThreadInfo));
    
    threadInfo.eip = InitialIP;
    threadInfo.esp = InitialSP;
    threadInfo.ebp = InitialFP;
    
    threadInfo.cs = GDT_KERNEL_CODE;
    threadInfo.ds = GDT_KERNEL_DATA;
    threadInfo.es = GDT_KERNEL_DATA;
    threadInfo.fs = GDT_KERNEL_DATA;
    threadInfo.gs = GDT_KERNEL_DATA;
    threadInfo.ss = GDT_KERNEL_DATA;
    
    threadInfo.eflags = readEflags();
    
    SetPageDirectory(PageDirectory);
}

void x86ThreadContext::initializeForUsermode(Address UNUSED(InitialIP), Address UNUSED(InitialSP), Address UNUSED(InitialFP), BasePageDirectory* UNUSED(PageDirectory))
{
    //FIXME: implement...
}

void x86ThreadContext::SetPageDirectory(BasePageDirectory* pd)
{
    pdir = (x86PageDirectory*)pd;
    threadInfo.cr3 = CurrentHAL->GetPaging()->GetPhysicalAddress((Address)pdir);
}

BasePageDirectory* x86ThreadContext::GetPageDirectory()
{
    return pdir;
}

void x86ThreadContext::SaveThreadContextFromRegisters(registers_t* state)
{
    if(comingFromUsermode(state->cs))
    {
        threadInfo.ss = state->ss;
        threadInfo.esp = state->useresp;
    }
    else
    {
        threadInfo.ss = GDT_KERNEL_DATA;
        threadInfo.esp = state->esp + 0x14;
    }
    
    threadInfo.eip = state->eip;
    threadInfo.cs = state->cs;
    threadInfo.eflags = state->eflags;
    threadInfo.eax = state->eax;
    threadInfo.ecx = state->ecx;
    threadInfo.edx = state->edx;
    threadInfo.ebx = state->ebx;
    
    threadInfo.ebp = state->ebp;
    threadInfo.esi = state->esi;
    threadInfo.edi = state->edi;
    threadInfo.ds = state->ds;
    threadInfo.es = state->es;
    threadInfo.fs = state->fs;
    threadInfo.gs = state->gs;
    
    //CR3 should not change during execution, we are not going to save this register
}

void x86ThreadContext::SwitchToThisContext()
{
    switchToThread(&threadInfo);
}
