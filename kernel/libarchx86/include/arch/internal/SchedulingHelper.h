#ifndef _ARCHX86_SCHEDULINGHELPER_H
#define _ARCHX86_SCHEDULINGHELPER_H

#include <types.h>

namespace Arch
{
    typedef struct x86ThreadInfo
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
        uint32_t  cr3;       // 64
    };
    
    extern "C"  uint32_t    readEflags();
    extern "C"  void        switchToThread(x86ThreadInfo* info);
    
    extern "C"  void        writeStackPointer(Address a);
    extern "C"  Address     readStackPointer();
    extern "C"  void        writeBasePointer(Address a);
    extern "C"  Address     readBasePointer();
}

#endif
