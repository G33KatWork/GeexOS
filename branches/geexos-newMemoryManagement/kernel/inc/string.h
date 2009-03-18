#ifndef _STRING_H
#define _STRING_H
//****************************************************************************
//**
//**    [string.h]
//**    - Standard C String routines
//**
//****************************************************************************

#include <size_t.h>


// ************************************ Public ***********************************

char *strcpy(char *s1, const char *s2);
size_t strlen ( const char* str );
int strcmp (const char *s1, const char *s2);

void* memcpy(void *dest, const void *src, size_t count);
void *memset(void *dest, char val, size_t count);
unsigned short* memsetw(unsigned short *dest, unsigned short val, size_t count);

void itoa(unsigned i,unsigned base,char* buf);
void itoa_s(int i,unsigned base,char* buf);

//****************************************************************************
//**
//**    END [string.h]
//**
//****************************************************************************

#endif
