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

//============================================================================
//    INTERFACE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================
//============================================================================
//    INTERFACE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    INTERFACE STRUCTURES / UTILITY CLASSES
//============================================================================
//============================================================================
//    INTERFACE DATA DECLARATIONS
//============================================================================
//============================================================================
//    INTERFACE FUNCTION PROTOTYPES
//============================================================================

void DebugPutc (unsigned char c);
void DebugClrScr (const unsigned short c);
void DebugPuts (const char* str);
int DebugPrintf (const char* str, ...);
unsigned DebugSetColor (const unsigned c);
void DebugGotoXY (unsigned x, unsigned y);

//============================================================================
//    INTERFACE OBJECT CLASS DEFINITIONS
//============================================================================
//============================================================================
//    INTERFACE TRAILING HEADERS
//============================================================================
//****************************************************************************
//**
//**    END DebugDisplay.h
//**
//****************************************************************************
#endif
