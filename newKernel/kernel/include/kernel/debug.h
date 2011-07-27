#ifndef _DEBUG_H
#define _DEBUG_H

#include <halinterface/HAL.h>

//Comment out for on screen debugging
#define     SERIAL_DEBUG

#ifdef DEBUG

//Debugging of several submodules
//undef, if not wanted
#define EN_DEBUG_MSG_MAIN
#define EN_HAL_DEBUG_MSG
// #define EN_SLAB_ALLOCATOR_DEBUG_MSG
// #define EN_SLAB_CACHE_DEBUG_MSG
// #define EN_SLAB_LARGE_DEBUG_MSG
// #define EN_SLAB_SMALL_DEBUG_MSG
// #define EN_HAL_ACPI_DEBUG_MSG
#define EN_IO_MEMORY_MANAGER_DEBUG_MSG
// #define EN_SCHEDULER_DEBUG_MSG
// #define EN_TIMER_MGR_DEBUG_MSG
// #define EN_ELF_INFORMATION_DEBUG_MSG
// #define EN_VIRTUAL_MEMORY_MANAGER_DEBUG_MSG
// #define EN_VIRTUAL_MEMORY_SPACE_DEBUG_MSG
// #define EN_VIRTUAL_MEMORY_REGION_DEBUG_MSG
// #define EN_KERNEL_THREAD_STACK_DEBUG_MSG
// #define EN_STACK_DEBUG_MSG
// #define EN_ARCH_INTERRUPTS_DEBUG_MSG
// #define EN_ARCH_PAGING_DEBUG_MSG
// #define EN_PLACEMENT_DEBUG_MSG
// #define EN_PHYS_BITFIELD_DEBUG_MSG
// #define EN_GDBSTUB_DEBUG_MSG
// #define EN_BITFIELD_DEBUG_MSG
// #define EN_BUDDY_ALLOC_DEBUG_MSG

#define DEBUG_MSG(msg) \
{ \
    Debug::BaseOutputDevice* kdbg = Arch::CurrentHAL->GetCurrentDebugOutputDevice(); \
    Debug::Color foreground = kdbg->GetForeground(); \
    kdbg->SetForeground(Debug::LightBlue); \
    kdbg->PrintString("[DEBUG] "); \
    *kdbg << msg << Debug::endl; \
    kdbg->SetForeground(foreground); \
}
#else
#define DEBUG_MSG(msg)
#endif


#ifdef EN_DEBUG_MSG_MAIN
    #define MAIN_DEBUG_MSG(x)                   DEBUG_MSG("MAIN: "x)
#else
    #define MAIN_DEBUG_MSG(x)
#endif

#ifdef EN_HAL_DEBUG_MSG
    #define HAL_DEBUG_MSG(x)                    DEBUG_MSG("HAL: "x)
#else
    #define HAL_DEBUG_MSG(x)
#endif

#ifdef EN_SLAB_ALLOCATOR_DEBUG_MSG
    #define SLAB_ALLOCATOR_DEBUG_MSG(x)         DEBUG_MSG("SLAB ALLOC: "x)
#else
    #define SLAB_ALLOCATOR_DEBUG_MSG(x)
#endif

#ifdef EN_SLAB_CACHE_DEBUG_MSG
    #define SLAB_CACHE_DEBUG_MSG(x)             DEBUG_MSG("SLAB CACHE: "x)
#else
    #define SLAB_CACHE_DEBUG_MSG(x)
#endif

#ifdef EN_SLAB_LARGE_DEBUG_MSG
    #define SLAB_LARGE_DEBUG_MSG(x)             DEBUG_MSG("LARGE CACHE: "x)
#else
    #define SLAB_LARGE_DEBUG_MSG(x)
#endif

#ifdef EN_SLAB_SMALL_DEBUG_MSG
    #define SLAB_SMALL_DEBUG_MSG(x)             DEBUG_MSG("SMALL CACHE: "x)
#else
    #define SLAB_SMALL_DEBUG_MSG(x)
#endif

#ifdef EN_HAL_ACPI_DEBUG_MSG
    #define HAL_ACPI_DEBUG_MSG(x)               DEBUG_MSG("HAL ACPI: "x)
#else
    #define HAL_ACPI_DEBUG_MSG(x)
#endif

#ifdef EN_IO_MEMORY_MANAGER_DEBUG_MSG
    #define IO_MEMORY_MANAGER_DEBUG_MSG(x)      DEBUG_MSG("IOMEM_MANAGER: "x)
#else
    #define IO_MEMORY_MANAGER_DEBUG_MSG(x)
#endif

#ifdef EN_SCHEDULER_DEBUG_MSG
    #define SCHEDULER_DEBUG_MSG(x)              DEBUG_MSG("SCHEDULER: "x)
#else
    #define SCHEDULER_DEBUG_MSG(x)
#endif

#ifdef EN_TIMER_MGR_DEBUG_MSG
    #define TIMER_MGR_DEBUG_MSG(x)              DEBUG_MSG("TIMER MGR: "x)
#else
    #define TIMER_MGR_DEBUG_MSG(x)
#endif

#ifdef EN_ELF_INFORMATION_DEBUG_MSG
    #define ELF_INFORMATION_DEBUG_MSG(x)        DEBUG_MSG("ELF_INFO: "x)
#else
    #define ELF_INFORMATION_DEBUG_MSG(x)
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

#ifdef EN_VIRTUAL_MEMORY_REGION_DEBUG_MSG    
    #define VIRTUAL_MEMORY_REGION_DEBUG_MSG(x)  DEBUG_MSG("VMEM_REGION: "x)
#else
    #define VIRTUAL_MEMORY_REGION_DEBUG_MSG(x)
#endif

#ifdef EN_STACK_DEBUG_MSG    
    #define STACK_DEBUG_MSG(x)                  DEBUG_MSG("STACK: "x)
#else
    #define STACK_DEBUG_MSG(x)
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

#ifdef EN_PLACEMENT_DEBUG_MSG
    #define PLACEMENT_DEBUG_MSG(x)              DEBUG_MSG("MEM_PLACEMENT: "x)
#else
    #define PLACEMENT_DEBUG_MSG(x)
#endif

#ifdef EN_PHYS_BITFIELD_DEBUG_MSG
    #define PHYS_BITFIELD_DEBUG_MSG(x)          DEBUG_MSG("PHYS_BITFIELD: "x)
#else
    #define PHYS_BITFIELD_DEBUG_MSG(x)
#endif

#ifdef EN_GDBSTUB_DEBUG_MSG
    #define GDBSTUB_DEBUG_MSG(x)                DEBUG_MSG("GDBSTUB: "x)
#else
    #define GDBSTUB_DEBUG_MSG(x)
#endif

#ifdef EN_KERNEL_THREAD_STACK_DEBUG_MSG
    #define KERNEL_THREAD_STACK_DEBUG_MSG(x)    DEBUG_MSG("KTSTACK: "x)
#else
    #define KERNEL_THREAD_STACK_DEBUG_MSG(x)
#endif

#ifdef EN_X86EMU_DEBUG_MSG
    #define X86EMU_DEBUG_MSG(x)                 DEBUG_MSG("X86EMU: "x)
#else
    #define X86EMU_DEBUG_MSG(x)
#endif

#ifdef EN_BITFIELD_DEBUG_MSG
    #define BITFIELD_DEBUG_MSG(x)               DEBUG_MSG("BITFIELD: "x)
#else
    #define BITFIELD_DEBUG_MSG(x)
#endif

#ifdef EN_BUDDY_ALLOC_DEBUG_MSG
    #define BUDDY_ALLOC_DEBUG_MSG(x)            DEBUG_MSG("BUDDY: "x)
#else
    #define BUDDY_ALLOC_DEBUG_MSG(x)
#endif



#endif
