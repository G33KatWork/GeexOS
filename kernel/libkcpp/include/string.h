#ifndef _STRING_H
#define _STRING_H

#include <types.h>

//namespace Lib
#ifdef __cplusplus
extern "C"
{
#endif
    void *memcpy(void *dest, const void* src, size_t count);
    void *memset(void *dest, char val, size_t count);
    void *memmove(void *dest, const void *src, size_t count);
    int memcmp(const void *_a, const void *_b, size_t count);

    size_t strlen(const char *str);
    size_t strnlen(const char *str, size_t count);
    int strcmp(const char *s1, const char *s2);
    int strncmp(const char *s1, const char *s2, size_t count);
    int strcasecmp(const char *s1, const char *s2);
    int strncasecmp(const char *s1, const char *s2, size_t count);

    char *strsep(char **stringp, const char *delim);
    char *strchr(const char *s, char c);
    char *strrchr(const char *s, int c);
    char *strcpy(char *dest, const char *src);
    char *strncpy(char *dest, const char *src, size_t count);
    char * strncat(char *dst, const char *src, size_t n);
    
    unsigned long strtoul(const char *nptr, char **endptr, int base);
#ifdef __cplusplus
}
#endif

#endif
