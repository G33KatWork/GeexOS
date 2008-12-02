typedef unsigned long vintp; //- integer type to store a pointer.

extern vintp start_ctors;
extern vintp end_ctors;
extern vintp start_dtors;
extern vintp end_dtors;

void Exit () {
	for(vintp * call = &start_dtors; call < &end_dtors; call++) {
	        ((void (*)(void))*call)();
	    }
}

void InitializeConstructors() {
	for(vintp * call = &start_ctors; call < &end_ctors; call++) {
	        ((void (*)(void))*call)();
	    }
}

//! purecall function handler
extern "C" void __cxa_pure_virtual()
{
	for (;;) ;
}

//! global new and delete operators
void* operator new (unsigned int size) throw() { return 0; }
void* operator new[] (unsigned int size) throw() { return 0; }
void operator delete (void * p) throw() {}
void operator delete[] (void * p) throw() { }

