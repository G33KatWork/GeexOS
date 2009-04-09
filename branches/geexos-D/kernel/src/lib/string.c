#include <string.h>

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
