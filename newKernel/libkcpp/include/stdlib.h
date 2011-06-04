#ifndef _STDLIB_H
#define _STDLIB_H

#include <types.h>

//namespace Lib
#ifdef __cplusplus
extern "C"
{
#endif
    int abs(int i);
    char* itoa(int value, char * str, int base);
#ifdef __cplusplus
}
#endif

#endif
