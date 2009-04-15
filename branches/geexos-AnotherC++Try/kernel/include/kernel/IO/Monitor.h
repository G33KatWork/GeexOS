#ifndef MONITOR_H_
#define MONITOR_H_

#include <lib/types.h>

namespace IO
{
    enum Color
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
    };
    
    enum Special
    {
        endl,
        hex,
        dec
    };

    class Monitor
    {
    public:
        Monitor();
        ~Monitor();
        
        void SetForeground(Color c)
        {
            foregroundColor = c;
        }
        
        void SetBackground(Color c)
        {
            backgroundColor = c;
        }
        
        Color GetForeground()
        {
            return (Color)foregroundColor;
        }
        
        Color GetBackground()
        {
            return (Color)backgroundColor;
        }
        
        void PrintChar(char c);
        
        void PrintString(char *c);
        void PrintString(const char *c)
        {
            PrintString((char*)c);  
        }
        
        void PrintHex(unsigned int n);
        void PrintDec(unsigned int n);
        void PrintDec(int n);
        
        void Clear();
        void Clear(Color c);
        
        Monitor &operator<<(char *c);
        Monitor &operator<<(const char *c);
        Monitor &operator<<(unsigned int i);
        Monitor &operator<<(int i);
        Monitor &operator<<(Special s);
        
        void Test(void);
    private:
        int foregroundColor;
        int backgroundColor;
        
        unsigned short *vidmem;
        unsigned int cursorX;
        unsigned int cursorY;
        Special printMode;
        
        void scroll();
        void moveCursor();
        
        void reverseArray(char* arr);
    };
}
#endif
