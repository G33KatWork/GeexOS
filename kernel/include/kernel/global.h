#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <lib/types.h>
#include <kernel/debug.h>
//#include <kernel/Memory/Virtual/VirtualMemoryManager.h>

//extern class Memory::VirtualMemoryManager memoryManager;

#define ASSERT(b, msg)      ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b, #msg))
#define PANIC(msg)          panic(msg)

#ifdef UNUSED 
#elif defined(__GNUC__) 
# define UNUSED(x) UNUSED_ ## x __attribute__((unused)) 
#elif defined(__LCLINT__) 
# define UNUSED(x) /*@unused@*/ x 
#else 
# define UNUSED(x) x 
#endif

extern void panic(const char *message);
void panic_assert(const char *file, unsigned int line, const char *condition, const char *desc);

//Standard heap allocation
void *operator new(size_t size);
void *operator new[](size_t size);
void  operator delete(void *p);
void  operator delete[](void *p);

//Operators for (de-)allocating page aligned 4KB memory regions needed by Paging mechanism.
//Be sure to deallocate this memory with the appropriate delete afterwards!
void *operator new(size_t size, bool pageAllocation);
void operator delete(void *p, bool pageAllocation);

void* kmalloc(size_t size);
void kfree(void* p);

unsigned int getPlacementPointer();

#endif
