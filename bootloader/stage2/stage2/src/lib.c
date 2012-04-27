#include <lib.h>
#include <heap.h>
#include <arch.h>

static Heap* default_heap = NULL;

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

char* strcpy(char *dest, const char *src)
{
	char *d = dest;

	while((*d++ = *src++));
	return dest;
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
    size_t i;
    
    if(d <= s || d >= s+count)
    {
        for(i = 0; i < count; i++) {
    		*d++ = *s++;
    	}
    }
    else
    {
        d = (char *)dest + count - 1;
        s = (const char *)s + count - 1;
        
        while(count > 0)
        {
            for(i = 0; i < count; i++) {
        		*d-- = *s--;
        	}
        }
    }
    
    return dest;
}
