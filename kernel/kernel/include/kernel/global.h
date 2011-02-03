#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <kernel/debug.h>

#define ASSERT(b, msg)      ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b, #msg))
//#define PANIC(msg)          panic(msg)

//FIXME: panic message should not go over debug output...
#define PANIC(msg) \
{ \
    Debug::BaseDebugOutputDevice* kdbg = Arch::CurrentHAL->GetCurrentDebugOutputDevice(); \
    kdbg->SetForeground(Debug::Red); \
    kdbg->PrintString("[PANIC] Kernel Panic <"__FILE__">: "); \
    *kdbg << msg << Debug::endl; \
	doPanic(); \
}

#ifdef UNUSED 
#elif defined(__GNUC__) 
# define UNUSED(x) UNUSED_ ## x __attribute__((unused)) 
#elif defined(__LCLINT__) 
# define UNUSED(x) /*@unused@*/ x 
#else 
# define UNUSED(x) x 
#endif

extern void doPanic();
void panic_assert(const char *file, unsigned int line, const char *condition, const char *desc);

//Standard heap allocation
//First this is the placement allocator
//and later this calls the slab allocator
void *operator new(size_t size);
void *operator new[](size_t size);
void  operator delete(void *p);
void  operator delete[](void *p);

//Allocation for objects in a preallocated buffer
void *operator new(size_t size, void* buffer);
void *operator new[](size_t size, void* buffer);

//Operators for (de-)allocating page aligned 4KB memory regions needed by Paging mechanism.
//Be sure to deallocate this memory with the appropriate delete afterwards!
void *operator new(size_t size, bool pageAllocation);
void operator delete(void *p, bool pageAllocation);

void* kmalloc(size_t size);
void kfree(void* p);
//void* krealloc(void* p, size_t s);


unsigned int getPlacementPointer();

#endif
