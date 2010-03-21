#ifndef _DEBUG_H
#define _DEBUG_H

#include <kernel/IO/Monitor.h>
#include <kernel/IO/SerialConsole.h>

//Comment out for on screen debugging
//#define     SERIAL_DEBUG

#ifdef SERIAL_DEBUG
    extern class IO::SerialConsole kdbg;
#else
    extern class IO::Monitor kdbg;
#endif

#ifdef DEBUG

//Debugging of several submodules
//undef, if not wanted
#define EN_DEBUG_MSG_MAIN
/*#define EN_VIRTUAL_MEMORY_MANAGER_DEBUG_MSG
#define EN_VIRTUAL_MEMORY_SPACE_DEBUG_MSG
#define EN_ARCH_INTERRUPTS_DEBUG_MSG
#define EN_ARCH_PAGING_DEBUG_MSG*/

#define DEBUG_MSG(msg) \
{ \
    Color foreground = kdbg.GetForeground(); \
    kdbg.SetForeground(LightBlue); \
    kdbg.PrintString("[DEBUG] "); \
    kdbg << msg << endl; \
    kdbg.SetForeground(foreground); \
}
#else
#define DEBUG_MSG(msg)
#endif


#ifdef EN_DEBUG_MSG_MAIN
    #define     MAIN_DEBUG_MSG(x)               DEBUG_MSG("MAIN: "x)
#else
    #define     MAIN_DEBUG_MSG(x)
#endif

#ifdef EN_VIRTUAL_MEMORY_MANAGER_DEBUG_MSG
    #define VIRTUAL_MEMORY_MANAGER_DEBUG_MSG(x) DEBUG_MSG("VMEM_MANAGER: "x)
#else
    #define VIRTUAL_MEMORY_MANAGER_DEBUG_MSG(x)
#endif

#ifdef EN_VIRTUAL_MEMORY_SPACE_DEBUG_MSG    
    #define VIRTUAL_MEMORY_SPACE_DEBUG_MSG(x)   DEBUG_MSG("VMEM_SPACE: "x)
#else
    #define VIRTUAL_MEMORY_SPACE_DEBUG_MSG(x)
#endif

#ifdef EN_ARCH_INTERRUPTS_DEBUG_MSG
    #define ARCH_INTERRUPTS_DEBUG_MSG(x)        DEBUG_MSG("ARCH_IRQ: "x)
#else
    #define ARCH_INTERRUPTS_DEBUG_MSG(x)
#endif

#ifdef EN_ARCH_PAGING_DEBUG_MSG
    #define ARCH_PAGING_DEBUG_MSG(x)            DEBUG_MSG("ARCH_PAGING: "x)
#else
    #define ARCH_PAGING_DEBUG_MSG(x)
#endif

#endif
