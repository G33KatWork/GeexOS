#include <arch/TextmodeDebugOutput.h>

#include <string.h>
#include <arch/internal/ports.h>
#include <arch/addressLayout.h>

using namespace Arch;

TextmodeDebugOutput::TextmodeDebugOutput()
{
    vidmem = (unsigned short *)(KERNEL_BASE + 0xB8000);
    cursorX = 0;
    cursorY = 0;
}

TextmodeDebugOutput::~TextmodeDebugOutput()
{
}

void TextmodeDebugOutput::PrintChar(char c)
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
        location = vidmem + (cursorY*80 + cursorX);
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

void TextmodeDebugOutput::Clear()
{
    cursorX = 0;
    cursorY = 0;
    
    unsigned char attributeByte = (backgroundColor << 4) | (foregroundColor & 0x0F);
    unsigned short blank = 0x20 /* space */ | (attributeByte << 8);
    
    for(int i = 0; i < 25*80; i++)
        vidmem[i] = blank;
    
    moveCursor();
}

void TextmodeDebugOutput::scroll()
{
    unsigned char attributeByte = (backgroundColor << 4) | (foregroundColor & 0x0F);
    unsigned short blank = 0x20 /* space */ | (attributeByte << 8);
    
    if(cursorY >= 25)
    {
        for(int i = 0; i < 24*80; i++)
            vidmem[i] = vidmem[i+80];
        
        //move all lines one line up and discard upper line
        //memmove(vidmem, vidmem + 80, 24*80);
        
        //clear lowest line
        for (int i = 24*80; i < 25*80; i++)
            vidmem[i] = blank;
        
        cursorY = 24;
    }
}

void TextmodeDebugOutput::moveCursor()
{
    unsigned short temp = cursorY * 80 + cursorX;
    outb(0x3D4, 14);
    outb(0x3D5, temp >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, temp);
}
