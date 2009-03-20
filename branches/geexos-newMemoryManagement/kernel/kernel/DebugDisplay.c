//****************************************************************************
//**
//**    DebugDisplay.c
//**	- Allows displaying debugging information for testing without
//**	worry of compatability nor hardware abstraction.
//**
//****************************************************************************

#include <stdarg.h>
#include <string.h>
#include "DebugDisplay.h"


// *********************************** Private ***********************************

// video memory
//static uint16_t *video_memory = (uint16_t *)0xB8000;
static uint16_t *video_memory = (uint16_t *)0xC00B8000;

// current position
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

// current color
static uint8_t	_color=0;


// ************************************ Public ***********************************

// Displays a character
void DebugPutc (unsigned char c) {

    uint16_t attribute = _color << 8;

    //! backspace character
    if (c == 0x08 && cursor_x)
        cursor_x--;

    //! tab character
    else if (c == 0x09)
        cursor_x = (cursor_x+8) & ~(8-1);

    //! carriage return
    else if (c == '\r')
        cursor_x = 0;

    //! new line
	else if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
	}

    //! printable characters
    else if(c >= ' ') {

		//! display character on screen
        uint16_t* location = video_memory + (cursor_y*80 + cursor_x);
        *location = c | attribute;
        cursor_x++;
    }

    //! if we are at edge of row, go to new line
    if (cursor_x >= 80) {

        cursor_x = 0;
        cursor_y++;
    }
}

// Sets new font color
unsigned DebugSetColor (const unsigned c) {

	unsigned t=_color;
	_color=c;
	return t;
}

// Sets new position
void DebugGotoXY (unsigned x, unsigned y) {

	if (cursor_x <= 80)
	    cursor_x = x;

	if (cursor_y <= 25)
	    cursor_y = y;
}

// Clear screen
void DebugClrScr (const unsigned short c) {

	// clear video memory by writing space characters to it
	for (int i = 0; i < 80*25; i++)
        video_memory[i] = ' ' | (c << 8);

    // move position back to start
    DebugGotoXY (0,0);
}

// Displays a string
void DebugPuts (const char* str) {

	if (!str)
		return;

	//! err... displays a string
    for (unsigned int i=0; i<strlen(str); i++)
        DebugPutc (str[i]);
}

// Displays a formatted string
int DebugPrintf (const char* str, ...) {

	if(!str)
		return 0;

	va_list		args;
	va_start (args, str);
	size_t i;
	for (i=0; i<strlen(str);i++) {

		switch (str[i]) {

			case '%':

				switch (str[i+1]) {

					/*** characters ***/
					case 'c': {
						char c = (char) va_arg (args, int);
						DebugPutc (c);
						i++;		// go to next character
						break;
					}

					/*** address of ***/
					case 's': {
						int* c = (int*) va_arg (args, int);
						char str2[64];
						strcpy (str2,(const char*)c);
						DebugPuts (str2);
						i++;		// go to next character
						break;
					}

					/*** integers ***/
					case 'd':
					case 'i': {
						int c = va_arg (args, int);
						char str2[32]={0};
						itoa_s (c, 10, str2);
						DebugPuts (str2);
						i++;		// go to next character
						break;
					}

					case 'u': {
						int c = va_arg (args, unsigned);
						char str2[32]={0};
						itoa (c, 10, str2);
						DebugPuts (str2);
						i++;		// go to next character
						break;
					}

					/*** display in hex ***/
					case 'X':
					case 'x': {
						int c = va_arg (args, int);
						char str2[32]={0};
						itoa (c,16,str2);
						DebugPuts (str2);
						i++;		// go to next character
						break;
					}

					default:
						va_end (args);
						return 1;
				}

				break;

			default:
				DebugPutc (str[i]);
				break;
		}

	}

	va_end (args);
	return i;
}
