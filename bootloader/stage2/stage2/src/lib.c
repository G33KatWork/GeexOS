#include <lib.h>

void memset(void *dest, int val, size_t count)
{
    char *temp = (char *)dest;
	for(; count != 0; count--) *temp++ = val;
	//return dest;
}

size_t strlen(const char *str)
{
	size_t retval;
	for(retval = 0; *str != '\0'; str++) retval++;
	return retval;
}
