#include <arch/i386/print.h>
#include <arch/i386/portio.h>
#include <lib.h>

unsigned short *videoram = (unsigned short *) 0xb8000;
unsigned int cursorX = 0;
unsigned int cursorY = 0;

Color foregroundColor = White;
Color backgroundColor = Black;

void scroll(void);
void moveCursor(void);

void print_i386_setBackgroundColor(Color c)
{
    backgroundColor = c;
}

void print_i386_setForegroundColor(Color c)
{
    foregroundColor = c;
}

void print_i386_clear()
{
    cursorX = 0;
    cursorY = 0;
    
    unsigned char attributeByte = (backgroundColor << 4) | (foregroundColor & 0x0F);
    unsigned short blank = 0x20 /* space */ | (attributeByte << 8);
    
    for(int i = 0; i < 25*80; i++)
        videoram[i] = blank;
    
    moveCursor();
}

void print_i386_string(char* s)
{
    while(*s)
    {
        print_i386_char(*s);
        s++;
    }
}

void print_i386_char(char c)
{
    unsigned char attributeByte = (backgroundColor << 4) | (foregroundColor & 0x0F);
    unsigned short attribute = attributeByte << 8;
    unsigned short *location;
    
    //Backspace
    if(c == 0x08 && cursorX)
        cursorX--;
        
    //Tab
    else if(c == 0x09)
        cursorX = (cursorX+8) & ~(8-1);
    
    //Carriage Return
    else if(c == '\r')
        cursorX = 0;
    
    //New Line
    else if(c == '\n')
    {
        cursorX = 0;
        cursorY++;
    }
    
    //Normal character
    else if(c >= ' ')
    {
        location = videoram + (cursorY*80 + cursorX);
        *location = c | attribute;
        cursorX++;
    }
    
    //Handle end of line
    if (cursorX >= 80)
    {
        cursorX = 0;
        cursorY ++;
    }
    
    scroll();
    moveCursor();
}

void scroll()
{
    unsigned char attributeByte = (backgroundColor << 4) | (foregroundColor & 0x0F);
    unsigned short blank = 0x20 /* space */ | (attributeByte << 8);
    
    if(cursorY >= 25)
    {
        for(int i = 0; i < 24*80; i++)
            videoram[i] = videoram[i+80];
        
        //move all lines one line up and discard upper line
        //memmove(vidmem, vidmem + 80, 24*80);
        
        //clear lowest line
        for (int i = 24*80; i < 25*80; i++)
            videoram[i] = blank;
        
        cursorY = 24;
    }
}

void moveCursor()
{
    unsigned short temp = cursorY * 80 + cursorX;
    portio_outb(0x3D4, 14);
    portio_outb(0x3D5, temp >> 8);
    portio_outb(0x3D4, 15);
    portio_outb(0x3D5, temp);
}
