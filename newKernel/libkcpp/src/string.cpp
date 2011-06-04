#include <string.h>
#include <ctype.h>
#include <limits.h>

void */*Lib::*/memcpy(void *dest, const void* src, size_t count)
{
    char *d = (char *)dest;
	const char *s = (const char *)src;
	size_t i;

	for(i = 0; i < count; i++) {
		*d++ = *s++;
	}

	return dest;
}

void */*Lib::*/memset(void *dest, char val, size_t count)
{
    char *temp = (char *)dest;
	for(; count != 0; count--) *temp++ = val;
	return dest;
}

void */*Lib::*/memmove(void *dest, const void *src, size_t count)
{
	char *a = (char *)dest;
	const char *b = (const char *)src;

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

int /*Lib::*/memcmp(const void *_a, const void *_b, size_t count)
{
    char *a = (char*)_a;
    char *b = (char*)_b;

    while(count-- > 0) {
        if(*a++ != *b++) return 1;
    }
    return 0;
}

size_t /*Lib::*/strlen(const char *str)
{
	size_t retval;
	for(retval = 0; *str != '\0'; str++) retval++;
	return retval;
}

size_t /*Lib::*/strnlen(const char *str, size_t count)
{
	size_t retval;
	for(retval = 0; *str != '\0' && retval < count; str++) retval++;
	return retval;
}

int /*Lib::*/strcmp(const char *s1, const char *s2)
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

int /*Lib::*/strncmp(const char *s1, const char *s2, size_t count)
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

int /*Lib::*/strcasecmp(const char *s1, const char *s2)
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

int /*Lib::*/strncasecmp(const char *s1, const char *s2, size_t count)
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

char */*Lib::*/strsep(char **stringp, const char *delim)
{
	char *s;
	const char *spanp;
	int c, sc;
	char *tok;

	if((s = *stringp) == NULL)
		return NULL;

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

char */*Lib::*/strchr(const char *s, char c)
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

char */*Lib::*/strrchr(const char *s, int c)
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

char */*Lib::*/strcpy(char *dest, const char *src)
{
	char *d = dest;

	while((*d++ = *src++));
	return dest;
}

char */*Lib::*/strncpy(char *dest, const char *src, size_t count)
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

char * /*Lib::*/strncat(char *dst, const char *src, size_t n)
{
        if (n != 0) {
                char *d = dst;
                const char *s = src;

                while (*d != 0)
                        d++;
                do {
                        if ((*d = *s++) == 0)
                                break;
                        d++;
                } while (--n != 0);
                *d = 0;
  }
        return (dst);
}

unsigned long /*Lib::*/strtoul(const char *nptr, char **endptr, int base)
{
    const char *s;
    unsigned long acc, cutoff;
    int c;
    int neg, any, cutlim;

    s = nptr;
    do {
        c = (unsigned char) *s++;
    } while (isspace(c));
    
    if (c == '-')
    {
        neg = 1;
        c = *s++;
    }
    else
    {
        neg = 0;
        if (c == '+')
            c = *s++;
    }
    
    if ((base == 0 || base == 16) &&
    c == '0' && (*s == 'x' || *s == 'X'))
    {
        c = s[1];
        s += 2;
        base = 16;
    }
    
    if (base == 0)
        base = c == '0' ? 8 : 10;

    cutoff = ULONG_MAX / (unsigned long)base;
    cutlim = ULONG_MAX % (unsigned long)base;
    for (acc = 0, any = 0;; c = (unsigned char) *s++)
    {
        if (isdigit(c))
            c -= '0';
        else if (isalpha(c))
            c -= isupper(c) ? 'A' - 10 : 'a' - 10;
        else
            break;
        
        if (c >= base)
            break;
        
        if (any < 0)
            continue;
        
        if (acc > cutoff || (acc == cutoff && c > cutlim))
        {
            any = -1;
            acc = ULONG_MAX;
            //errno = ERANGE;
        }
        else
        {
            any = 1;
            acc *= (unsigned long)base;
            acc += c;
        }
    }
    
    if (neg && any > 0)
        acc = -acc;
    
    if (endptr != 0)
        *endptr = (char *) (any ? s - 1 : nptr);
    
    return (acc);
}
