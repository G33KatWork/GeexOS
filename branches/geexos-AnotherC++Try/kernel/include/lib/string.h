#ifndef _STRING_H
#define _STRING_H

#include <lib/types.h>

namespace Lib
{

    void *memcpy(void *dest, const void* src, size_t count);
    void *memset(void *dest, int val, size_t count);
    void *memmove(void *dest, const void *src, size_t count);
    void *memdup(const void *src, size_t count);

    size_t strlen(const char *str);
    size_t strnlen(const char *str, size_t count);
    int strcmp(const char *s1, const char *s2);
    int strncmp(const char *s1, const char *s2, size_t count);
    int strcasecmp(const char *s1, const char *s2);
    int strncasecmp(const char *s1, const char *s2, size_t count);

    char *strsep(char **stringp, const char *delim);
    char *strchr(const char *s, int c);
    char *strrchr(const char *s, int c);
    char *strcpy(char *dest, const char *src);
    char *strncpy(char *dest, const char *src, size_t count);
    char *strdup(const char *s);
    char *strndup(const char *s, size_t n);
}

#endif
