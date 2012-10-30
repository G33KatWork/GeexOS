#include <testkernellib/lib.h>
#include <testkernel/kernelapi.h>

void DllEntry()
{
	
}

int anotherfunction()
{
	return 16+kernelapifunc(1);
}

int libfunc(int a)
{
	return anotherfunction()* a + 4;
}
