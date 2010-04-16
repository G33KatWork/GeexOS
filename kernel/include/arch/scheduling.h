#ifndef _ARCH_SCHEDULING_H
#define _ARCH_SCHEDULING_H

#include <lib/types.h>
#include <kernel/global.h>
#include <kernel/debug.h>

namespace Arch
{
    typedef struct ThreadInfo
    {
        uint32_t  eip;       // 0
        uint32_t  cs;        // 4
        uint32_t  eflags;    // 8
        uint32_t  eax;       // 12
        uint32_t  ecx;       // 16
        uint32_t  edx;       // 20
        uint32_t  ebx;       // 24
        uint32_t  esp;       // 28
        uint32_t  ebp;       // 32
        uint32_t  esi;       // 36
        uint32_t  edi;       // 40
        uint32_t  ds;        // 44
        uint32_t  es;        // 48
        uint32_t  fs;        // 52
        uint32_t  gs;        // 56
        uint32_t  ss;        // 60
    };
    
    extern "C"  void    writeStackPointer(Address a);
    extern "C"  Address readStackPointer();
    extern "C"  void    writeBasePointer(Address a);
    extern "C"  Address readBasePointer();
	
    extern "C"  Address readInstructionPointer();
    
    extern "C"  void    switchToThread(ThreadInfo* info);
    
    static inline   void    saveThreadInfo(ThreadInfo* info, registers_t* oldState)
    {
        info->eip = oldState->eip;
        info->cs = oldState->cs;
        info->eflags = oldState->eflags;
        info->eax = oldState->eax;
        info->ecx = oldState->ecx;
        info->edx = oldState->edx;
        info->ebx = oldState->ebx;
        info->esp = oldState->esp + 0x14;
        info->ebp = oldState->ebp;
        info->esi = oldState->esi;
        info->edi = oldState->edi;
        info->ds = oldState->ds;
        info->es = oldState->es;
        info->fs = oldState->fs;
        info->gs = oldState->gs;
        /*info->ss = oldState->ss;*/
    }
    
    #define printThreadInfo(info) \
        SCHEDULER_DEBUG_MSG("ThreadInfo: "); \
        SCHEDULER_DEBUG_MSG("EIP: " << IO::hex << info->eip); \
        SCHEDULER_DEBUG_MSG("CS: " << IO::hex << info->cs); \
        SCHEDULER_DEBUG_MSG("EFLAGS: " << IO::hex << info->eflags); \
        SCHEDULER_DEBUG_MSG("EAX: " << IO::hex << info->eax); \
        SCHEDULER_DEBUG_MSG("ECX: " << IO::hex << info->ecx); \
        SCHEDULER_DEBUG_MSG("EDX: " << IO::hex << info->edx); \
        SCHEDULER_DEBUG_MSG("EBX: " << IO::hex << info->ebx); \
        SCHEDULER_DEBUG_MSG("ESP: " << IO::hex << info->esp); \
        SCHEDULER_DEBUG_MSG("EBP: " << IO::hex << info->ebp); \
        SCHEDULER_DEBUG_MSG("ESI: " << IO::hex << info->esi); \
        SCHEDULER_DEBUG_MSG("EDI: " << IO::hex << info->edi); \
        SCHEDULER_DEBUG_MSG("DS: " << IO::hex << info->ds); \
        SCHEDULER_DEBUG_MSG("ES: " << IO::hex << info->es); \
        SCHEDULER_DEBUG_MSG("FS: " << IO::hex << info->fs); \
        SCHEDULER_DEBUG_MSG("GS: " << IO::hex << info->gs); \
        SCHEDULER_DEBUG_MSG("SS: " << IO::hex << info->ss);
}

#endif
