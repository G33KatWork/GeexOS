#ifndef _HAL_INTERFACES_BASE_OUTPUT_DEVICE_H
#define _HAL_INTERFACES_BASE_OUTPUT_DEVICE_H

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
        White=15
    };
    
    //Special control characters
    enum Special
    {
        endl,
        hex,
        dec
    };
    
    //Type of debug output device
    enum DebugOutputDeviceType
    {
        None,
        Serial
        //TODO: Add GDB/Debugger/Firewire/USB/UDP/whatnot output
    };
}

namespace Debug
{   
    //Baseclass for simple character output devices
    class BaseOutputDevice
    {
    protected:
		int foregroundColor;
	    int backgroundColor;
	    Special printMode;
	
		BaseOutputDevice()
		{
		    foregroundColor = White;
            backgroundColor = Black;
            printMode = hex;
		}
        virtual ~BaseOutputDevice() {};

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
        
        BaseOutputDevice &operator<<(char *c);
        BaseOutputDevice &operator<<(const char *c);
        BaseOutputDevice &operator<<(unsigned int i);
        BaseOutputDevice &operator<<(int i);
        BaseOutputDevice &operator<<(Special s);
        
		virtual void Clear() = 0;
        virtual void PrintChar(char c) = 0;
    };
    
    class NullDebugOutputDevice : public BaseOutputDevice
    {
    public:
        NullDebugOutputDevice() {}
        ~NullDebugOutputDevice() {}

        //BaseOutputDevice Methods
        void PrintChar(char c __attribute__((unused))) {}
        void Clear() {}
    };
}

#endif
