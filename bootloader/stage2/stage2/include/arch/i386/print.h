#ifndef _PRINT_I386_H_
#define _PRINT_I386_H_

#include <types.h>

typedef enum
{
    Black=0,
    Blue=1,
    Green=2,
    Cyan=3,
    Red=4,
    Magenta=5,
    Orange=6,
    LightGrey=7,
    DarkGrey=8,
    LightBlue=9,
    LightGreen=10,
    LightCyan=11,
    LightRed=12,
    LightMagenta=13,
    Yellow=14,
    White=15,
    None=16
} Color;

void print_i386_clear(void);
void print_i386_string(char* s);
void print_i386_char(char c);
void print_i386_setBackgroundColor(Color c);
void print_i386_setForegroundColor(Color c);

#endif
