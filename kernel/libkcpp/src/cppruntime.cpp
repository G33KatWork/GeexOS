//#include <kernel/global.h>
#include <string.h>
#include <cxxabi.h>

//C++ runtime shit
//See: http://wiki.osdev.org/C_PlusPlus

//Support for pure virtual functions
extern "C" void __cxa_pure_virtual()
{
    //PANIC("Error while calling pure virtual function!");
}

//__cxa_atexit and __cxa_finalize implementations

//maximum static objects
#define NUM_MAX_STATIC_OBJECTS      32

extern "C"
{
    int __cxa_atexit(void (*f)(void *), void *p, void *d);
    void __cxa_finalize(void *d);
}
 
void *__dso_handle; /*only the address of this symbol is taken by gcc*/
 
typedef struct
{
    void (*f)(void*);
    void *p;
    void *d;
} objects;
objects object[NUM_MAX_STATIC_OBJECTS];

unsigned int iObject = 0;
 
int __cxa_atexit(void (*f)(void *), void *p, void *d)
{
    //initialize everything to 0
    if(iObject == 0)
        memset(object, NULL, sizeof(object));
    
    if (iObject >= NUM_MAX_STATIC_OBJECTS) return -1;
    object[iObject].f = f;
    object[iObject].p = p;
    object[iObject].d = d;
    ++iObject;
    return 0;
}
 
/* This currently destroys all objects */
void __cxa_finalize(void*)
{
    unsigned int i = iObject;
    for (; i > 0; --i)
    {
        --iObject;
        
        if(object[iObject].f != NULL)
            object[iObject].f(object[iObject].p);
    }
}

namespace __cxxabiv1 
{
    //FIXME: Use a real guard, e.g. a Mutex or Semaphore, here!
	/* guard variables */
 
	extern "C" int __cxa_guard_acquire (__guard *g) 
	{
		return !*(char *)(g);
	}
 
	extern "C" void __cxa_guard_release (__guard *g)
	{
		*(char *)g = 1;
	}
 
	extern "C" void __cxa_guard_abort (__guard *)
	{
	}
}
