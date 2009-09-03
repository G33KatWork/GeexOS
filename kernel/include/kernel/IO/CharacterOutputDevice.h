#include <lib/types.h>

#ifndef CHARACTER_OUTPUT_DEVICE_H
#define CHARACTER_OUTPUT_DEVICE_H

namespace IO
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
    
    //Baseclass for Character output devices
    class CharacterOutputDevice
    {
    private:
        void reverseArray(char* arr);
        
    protected:
		int foregroundColor;
	    int backgroundColor;
		Special printMode;
	
		CharacterOutputDevice();
        virtual ~CharacterOutputDevice() {};

    public:    
        //Color management
        void SetForeground(Color c) { foregroundColor = c; }
        void SetBackground(Color c) { backgroundColor = c; }
        Color GetForeground() { return (Color)foregroundColor; }
        Color GetBackground() { return (Color)backgroundColor; }
        
        void PrintString(char *c);
		void PrintString(const char *c)
        {
            PrintString((char*)c);  
        }
		
		void PrintHex(unsigned int n);
        void PrintDec(unsigned int n);
        void PrintDec(int n);
        
        void Clear(Color c);

		CharacterOutputDevice &operator<<(char *c);
        CharacterOutputDevice &operator<<(const char *c);
        CharacterOutputDevice &operator<<(unsigned int i);
        CharacterOutputDevice &operator<<(int i);
        CharacterOutputDevice &operator<<(Special s);
		
		//Low level functions that need to be implemented by the child class
		virtual void Clear() = 0;
        virtual void PrintChar(char c) = 0;
    };
}

#endif
