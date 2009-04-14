#include <lib/string.h>
#include <lib/ctype.h>
#include <kernel/kmalloc.h>

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

void *memset(void *dest, int val, size_t count)
{
	char *temp = (char *)dest;
	for(; count != 0; count--) *temp++ = val;
	return dest;
}

void *memmove(void *dest, const void *src, size_t count)
{
	char *a = dest;
	const char *b = src;

	if(src != dest)
	{
		if(src > dest)
		{
			while(count--)
				*a++ = *b++;
		}
		else
		{
			a += count - 1;
			b += count - 1;
			while(count--)
				*a-- = *b--;
		}
	}

	return dest;
}

void *memdup(const void *src, size_t count)
{
	char *dest;

	if(count == 0)
		return NULL;

	dest = kmalloc(count);
	if(dest == NULL)
		return NULL;

	memcpy(dest, src, count);
	return dest;
}

size_t strlen(const char *str)
{
	size_t retval;
	for(retval = 0; *str != '\0'; str++) retval++;
	return retval;
}

size_t strnlen(const char *str, size_t count)
{
	size_t retval;
	for(retval = 0; *str != '\0' && retval < count; str++) retval++;
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

int strncmp(const char *s1, const char *s2, size_t count)
{
	const char *a = s1;
	const char *b = s2;
	const char *fini = a + count;

	while(a < fini)
	{
		int res = *a - *b;
		if(res)
			return res;
		if(!*a)
			return 0;
		a++; b++;
	}
	return 0;
}

int strcasecmp(const char *s1, const char *s2)
{
	for(;;)
	{
		if(!*s2 || (tolower(*s1) != tolower(*s2)))
			break;
		s1++;
		s2++;
	}
	return tolower(*s1) - tolower(*s2);
}

int strncasecmp(const char *s1, const char *s2, size_t count)
{
	const char *a = s1;
	const char *b = s2;
	const char *fini = a + count;

	while(a < fini)
	{
		int res = tolower(*a) - tolower(*b);
		if(res)
			return res;
		if(!*a)
			return 0;
		a++; b++;
	}
	return 0;
}

char *strsep(char **stringp, const char *delim)
{
	char *s;
	const char *spanp;
	int c, sc;
	char *tok;

	if((s = *stringp) == NULL)
		return (NULL);

	for(tok = s;;)
	{
		c = *s++;
		spanp = delim;
		do
		{
			if((sc = *spanp++) == c)
			{
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;

				*stringp = s;
				return (tok);
			}
		} while(sc != 0);
	}
}

char *strchr(const char *s, int c)
{
	char ch = c;

	for (;;)
	{
		if(*s == ch)
			break;
		else if(!*s)
			return NULL;
		else
			s++;
	}

	return (char *)s;
}

char *strrchr(const char *s, int c)
{
	const char *l = NULL;

	for(;;)
	{
		if(*s == c)
			l = s;
		if (!*s)
			return (char *)l;
		s++;
	}

	return (char *)l;
}

char *strcpy(char *dest, const char *src)
{
	char *d = dest;

	while((*d++ = *src++));
	return dest;
}

char *strncpy(char *dest, const char *src, size_t count)
{
	size_t i;
	
	for(i = 0; i < count; i++)
	{
		dest[i] = src[i];
		if(!src[i])
			break;
	}
	return dest;
}

char *strdup(const char *s)
{
	char *dup;
	size_t len = strlen(s) + 1;

	dup = kmalloc(len);
	if(dup == NULL)
		return NULL;

	memcpy(dup, s, len);
	return dup;
}

char *strndup(const char *s, size_t n)
{
	size_t len;
	char *dup;

	len = strlen(s);
	if(n < len)
        len = n;

	dup = kmalloc(len + 1);
	if(dup == NULL)
		return NULL;

	memcpy(dup, s, len);
	dup[len] = '\0';
	return dup;
}

int atoi(const char *s)
{
	int v = 0;
	int sign = 1;

	while(*s == ' ' || (unsigned int)(*s - 9) < 5u)
		s++;

	switch(*s)
	{
		case '-':
			sign=-1;
		case '+':
			s++;
	}

	while((unsigned int) (*s - '0') < 10u)
	{
		v = v * 10 + *s - '0';
		s++;
	}
	
	return (sign == -1)? -v:v;
}


