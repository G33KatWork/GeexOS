#include <lib.h>
#include <print.h>

void memset(void *dest, int val, size_t count)
{
    char *temp = (char *)dest;
	for(; count != 0; count--) *temp++ = val;
	//return dest;
}

void *memcpy(void *dest, const void* src, size_t count)
{
    char *d = (char *)dest;
	const char *s = (const char *)src;
	size_t i;

	for(i = 0; i < count; i++) {
		*d++ = *s++;
	}

	return dest;
}

size_t strlen(const char *str)
{
	size_t retval;
	for(retval = 0; *str != '\0'; str++) retval++;
	return retval;
}

int strcmp(const char *s1, const char *s2)
{
	char x;

	for(;;)
	{
		x = *s1;
		if(x != *s2)
			break;
		if(!x)
			break;
		s1++;
		s2++;
	}
	return x - *s2;
}

extern void cpuhalt(void);      //start.S
void panic(const char* message)
{
    print_set_foreground_color(White);
    print_set_background_color(Red);
    print_clear();
    print_string_static(message);
    cpuhalt();
}
