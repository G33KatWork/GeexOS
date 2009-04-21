#include <kernel/IO/Monitor.h>

#include <lib/string.h>
#include <arch/ports.h>

using namespace IO;
using namespace Lib;
using namespace Arch;

Monitor::Monitor()
{
    vidmem = (unsigned short *)0xC00B8000;
    cursorX = 0;
    cursorY = 0;
    printMode = dec;
    
    foregroundColor = White;
    backgroundColor = Black;
}

Monitor::~Monitor()
{
}

void Monitor::PrintChar(char c)
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

void Monitor::PrintString(char *c)
{
    unsigned int i = 0;
    while(c[i])
    {
        PrintChar(c[i]);
        i++;
    }
}

void Monitor::PrintHex(unsigned int n)
{
    char hexChars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    
    char num[20];
    memset(num, 0, 20);
    
    int i = 0;
    do
    {
        num[i] = hexChars[n % 16];
        n /= 16;
        
        i++;
    } while(n != 0);
    
    reverseArray(num);
    PrintString("0x");
    PrintString(num);
}

void Monitor::PrintDec(unsigned int n)
{
    char num[20];
    memset(num, 0, 20);
    
    int i = 0;
    do
    {
        num[i] = (n % 10) + '0';
        n /= 10;
        
        i++;
    } while(n != 0);
    
    reverseArray(num);
    PrintString(num);
}

void Monitor::PrintDec(int n)
{
    int sign;
    char num[20];
    memset(num, 0, 20);
    
    if((sign = n) < 0) n = -n;
    
    int i = 0;
    do
    {
        num[i] = (n % 10) + '0';
        n /= 10;
        
        i++;
    } while(n != 0);
    
    if(sign < 0)
        num[i] = '-';
    
    reverseArray(num);
    PrintString(num);
}

void Monitor::Clear()
{
    cursorX = 0;
    cursorY = 0;
    
    unsigned char attributeByte = (backgroundColor << 4) | (foregroundColor & 0x0F);
    unsigned short blank = 0x20 /* space */ | (attributeByte << 8);
    
    for(int i = 0; i < 25*80; i++)
        vidmem[i] = blank;
    
    moveCursor();
}

void Monitor::Clear(Color c)
{
    backgroundColor = c;
    Clear();
}

void Monitor::scroll()
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

void Monitor::moveCursor()
{
    unsigned short temp = cursorY * 80 + cursorX;
    outb(0x3D4, 14);
    outb(0x3D5, temp >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, temp);
}

Monitor &Monitor::operator<<(char *c)
{
    PrintString(c);
    return *this;
}

Monitor &Monitor::operator<<(const char *c)
{
    PrintString(c);
    return *this;
}

Monitor &Monitor::operator<<(unsigned int i)
{
    if (printMode == dec)
        PrintDec(i);
    else
        PrintHex(i);
    
    return *this;
}

Monitor &Monitor::operator<<(int i)
{
    if (printMode == dec)
        PrintDec(i);
    else
        PrintHex(i);
    
    return *this;
}

Monitor &Monitor::operator<<(Special s)
{
    switch(s)
    {
        case endl:
            PrintChar('\n');
            break;
        case dec:
            printMode = dec;
            break;
        case hex:
            printMode = hex;
            break;
      }
      
      return *this;
}


Monitor &Monitor::operator<<(Position p)
{
    cursorX = p.x;
    cursorY = p.y;
    
    scroll();
    moveCursor();
      
    return *this;
}

void Monitor::reverseArray(char* arr)
{
    int i, j;
    for (i = 0, j = strlen(arr)-1; i<j; i++, j--)
    {
        int c = arr[i];
        arr[i] = arr[j];
        arr[j] = c;
    }
}
