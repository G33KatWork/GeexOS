#include <kernel/global.h>

//C++ runtime shit
//See: http://wiki.osdev.org/C_PlusPlus

//Support for pure virtual functions
extern "C" void __cxa_pure_virtual()
{
    PANIC("Error while calling pure virtual function!");
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
 
struct object
{
    void (*f)(void*);
    void *p;
    void *d;
} object[NUM_MAX_STATIC_OBJECTS] = {0};
unsigned int iObject = 0;
 
int __cxa_atexit(void (*f)(void *), void *p, void *d)
{
    if (iObject >= NUM_MAX_STATIC_OBJECTS) return -1;
    object[iObject].f = f;
    object[iObject].p = p;
    object[iObject].d = d;
    ++iObject;
    return 0;
}
 
/* This currently destroys all objects */
void __cxa_finalize(void *d)
{
    unsigned int i = iObject;
    for (; i > 0; --i)
    {
        --iObject;
        object[iObject].f(object[iObject].p);
    }
}
