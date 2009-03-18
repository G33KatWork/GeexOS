#ifndef _DEBUGDISPLAY_H
#define _DEBUGDISPLAY_H
//****************************************************************************
//**
//**    DebugDisplay.h
//**    - Allows displaying debugging information for testing without
//**	worry of compatability nor hardware abstraction.
//**
//****************************************************************************
//============================================================================
//    INTERFACE REQUIRED HEADERS
//============================================================================

#include <stdint.h>

// ************************************ Public ***********************************

void DebugPutc (unsigned char c);
void DebugClrScr (const unsigned short c);
void DebugPuts (const char* str);
int DebugPrintf (const char* str, ...);
unsigned DebugSetColor (const unsigned c);
void DebugGotoXY (unsigned x, unsigned y);


//****************************************************************************
//**
//**    END DebugDisplay.h
//**
//****************************************************************************
#endif
