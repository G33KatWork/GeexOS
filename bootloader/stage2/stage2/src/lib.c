#include <lib.h>
#include <heap.h>
#include <arch.h>

Heap* default_heap = NULL;

void default_heap_init(void)
{
	if(default_heap)
		return;

	default_heap = heap_create(DEFAULT_HEAP_SIZE);
}

void* malloc(size_t len)
{
	if(!default_heap)
		arch_panic("Tried to malloc() without initialized heap");

	return heap_alloc(default_heap, len);
}

void free(void* p)
{
	if(!default_heap)
		arch_panic("Tried to free() without initialized heap");

	heap_free(default_heap, p);
}

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

char tolower(char c)
{
	if(('A' <= c) && (c <= 'Z'))
		c = 'a' + (c - 'A');

	return c;
}

char* strcpy(char *dest, const char *src)
{
	char *d = dest;

	while((*d++ = *src++));
	return dest;
}

char* strncpy(char *s1, const char *s2, size_t n)
{
	char *dest = s1;
	
	for (; *s2 && n; n--)
		*s1++ = *s2++;
	
	while (n--)
		*s1++ = 0;
	
	return dest;
}

int strcmp(const char *s1, const char *s2)
{
	while (*s1 != 0 && *s1 == *s2)
		s1++, s2++;

	if (*s1 == 0 || *s2 == 0)
		return (unsigned char) *s1 - (unsigned char) *s2;
	
	return *s1 - *s2;
}

int stricmp(const char *s1, const char *s2)
{
	while (*s1 != 0 && tolower(*s1) == tolower(*s2))
		s1++, s2++;

	if (*s1 == 0 || *s2 == 0)
		return (unsigned char) tolower(*s1) - (unsigned char) tolower(*s2);
	
	return tolower(*s1) - tolower(*s2);
}

int strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char u1, u2;

	while (n-- > 0)
	{
		u1 = (unsigned char) *s1++;
		u2 = (unsigned char) *s2++;
		if (u1 != u2)
			return u1 - u2;
		if (u1 == '\0')
			return 0;
	}
	return 0;
}

char* strchr(const char *s, int c)
{
	for (;;)
	{
		if (*s == c)
			return (char *) s;
		if (*s == 0)
			return 0;
		s++;
	}
}

char* strrchr(const char *s, int c)
{
	size_t i = 0;
	while (s[i] != 0)
		i++;

	do
		if (s[i] == c)
			return (char *) s + i;
	while (i-- != 0);

	return 0;
}

char* strcat(char *dst, const char *src)
{
    strcpy(strchr(dst, '\0'), src);
    return dst;
}

char* strncat(char *dst, const char *src, size_t n)
{
    strncpy(strchr(dst, '\0'), src, n);
    return dst;
}

void* memcpy(void *dest, const void* src, size_t count)
{
	char *d = (char *)dest;
	const char *s = (const char *)src;
	size_t i;

	for(i = 0; i < count; i++) {
		*d++ = *s++;
	}

	return dest;
}

void* memmove(void *dest, const void* src, size_t count)
{
	char *d = (char *)dest;
	const char *s = (const char *)src;
	
	if(d < s)
		while(count--)
			*d++ = *s++;
	else
	{
		d += count;
		s += count;
		
		while(count--)
			*d-- = *s--;
	}
	
	return dest;
}

int memcmp(const void *s1, const void *s2, size_t len)
{
	const unsigned char *sp1, *sp2;
	
	sp1 = s1;
	sp2 = s2;
	
	while (len != 0 && *sp1 == *sp2)
		sp1++, sp2++, len--;

	if (len == 0)
		return 0;
	
	return *sp1 - *sp2;
}
