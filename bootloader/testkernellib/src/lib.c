#include <testkernellib/lib.h>
#include <testkernel/kernelapi.h>

int globalvariable = 1;

void ctor(void) __attribute__((constructor));
void ctor()
{
	globalvariable = 23;
}

int anotherfunction()
{
	return 16+kernelapifunc(1);
}

int libfunc(int a)
{
	globalvariable++;
	return anotherfunction()* a + globalvariable;
}
