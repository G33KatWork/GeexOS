#include <print.h>
#include <lib.h>

unsigned short *videoram = (unsigned short *) 0xb8000;
unsigned int cursorX = 0;
unsigned int cursorY = 0;

Color foregroundColor = White;
Color backgroundColor = Black;

void print_set_foreground_color(Color c) { foregroundColor = c; }
void print_set_background_color(Color c) { backgroundColor = c; }

static void reverseArray(char* arr)
{
    int i, j;
    for (i = 0, j = strlen(arr)-1; i<j; i++, j--)
    {
        int c = arr[i];
        arr[i] = arr[j];
        arr[j] = c;
    }
}

void print_clear(void)
{
    cursorX = 0;
    cursorY = 0;
    
    unsigned char attributeByte = (backgroundColor << 4) | (foregroundColor & 0x0F);
    unsigned short blank = 0x20 /* space */ | (attributeByte << 8);
    
    for(int i = 0; i < 25*80; i++)
        videoram[i] = blank;
}

void print_string(char* s)
{
    while(*s)
    {
        print_char(*s);
        s++;
    }
}

void print_string_static(const char* s)
{
    print_string((char*)s);
}

void print_char(char c)
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
}

void print_integer(uint32_t n)
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
    print_string(num);
}

void print_integer_hex(uint32_t n)
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
	print_string_static("0x");
    print_string(num);
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
