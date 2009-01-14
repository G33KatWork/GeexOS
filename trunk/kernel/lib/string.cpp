
//****************************************************************************
//**
//**    [string.cpp]
//**    - [Standard C string routine implimentation]
//**
//****************************************************************************
//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

#include <string.h>

//============================================================================
//    IMPLEMENTATION PRIVATE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE STRUCTURES / UTILITY CLASSES
//============================================================================
//============================================================================
//    IMPLEMENTATION REQUIRED EXTERNAL REFERENCES (AVOID)
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE DATA
//============================================================================
//============================================================================
//    INTERFACE DATA
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTION PROTOTYPES
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTIONS
//============================================================================
//============================================================================
//    INTERFACE FUNCTIONS
//============================================================================

//! copies string s2 to s1
char *strcpy(char *s1, const char *s2)
{
    char *s1_p = s1;
    while ((*s1++ = *s2++));
    return s1_p;
}

//! returns length of string
size_t strlen ( const char* str ) {

	size_t	len=0;
	while (str[len++]);
	return len;
}

int strcmp (const char *s1, const char *s2)
{
	while (*s1 != 0 && *s1 == *s2)
		s1++, s2++;

	if (*s1 == 0 || *s2 == 0)
		return (unsigned char) *s1 - (unsigned char) *s2;

	return *s1 - *s2;
}


//! copies count bytes from src to dest
void *memcpy(void *dest, const void *src, size_t count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

//! sets count bytes of dest to val
void *memset(void *dest, char val, size_t count)
{
    unsigned char *temp = (unsigned char *)dest;
	for( ; count != 0; count--, temp[count] = val);
	return dest;
}

//! sets count bytes of dest to val
unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count)
{
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--)
		*temp++ = val;
    return dest;
}

//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[String.cpp]
//**
//****************************************************************************
