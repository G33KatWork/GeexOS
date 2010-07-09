#ifndef _HAL_INTERFACES_BASE_DEBUG_OUTPUT_DEVICE_H
#define _HAL_INTERFACES_BASE_DEBUG_OUTPUT_DEVICE_H

#include <types.h>

namespace Debug
{
    //Colors
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
    
    //Special control characters
    enum Special
    {
        endl,
        hex,
        dec
    };
}

namespace Debug
{   
    //Baseclass for Debugging output devices
    class BaseDebugOutputDevice
    {
    protected:
		int foregroundColor;
	    int backgroundColor;
	    Special printMode;
	
		BaseDebugOutputDevice()
		{
		    foregroundColor = White;
            backgroundColor = Black;
            printMode = hex;
		}
        virtual ~BaseDebugOutputDevice() {};

        void reverseArray(char* arr);

    public:    
        //Color management
        void SetForeground(Color c) { foregroundColor = c; }
        void SetBackground(Color c) { backgroundColor = c; }
        Color GetForeground() { return (Color)foregroundColor; }
        Color GetBackground() { return (Color)backgroundColor; }
        
        void PrintString(char *c);
        void PrintString(const char *c) { PrintString((char*)c); }
		void PrintHex(unsigned int n);
        void PrintDec(unsigned int n);
        void PrintDec(int n);
        void PrintData(char* start, size_t len);
        
        BaseDebugOutputDevice &operator<<(char *c);
        BaseDebugOutputDevice &operator<<(const char *c);
        BaseDebugOutputDevice &operator<<(unsigned int i);
        BaseDebugOutputDevice &operator<<(int i);
        BaseDebugOutputDevice &operator<<(Special s);
        
		virtual void Clear() = 0;
        virtual void PrintChar(char c) = 0;
    };
}

#endif
