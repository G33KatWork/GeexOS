#ifndef _PRINT_H
#define _PRINT_H

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

void print_clear(void);
void print_string(char* s);
void print_string_static(const char* s);
void print_char(char c);
void scroll(void);
void print_integer(uint32_t n);
void print_integer_hex(uint32_t n);
void print_set_foreground_color(Color c);
void print_set_background_color(Color c);

#endif
