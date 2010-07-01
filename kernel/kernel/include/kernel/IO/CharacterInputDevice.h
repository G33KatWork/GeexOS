#include <types.h>

#ifndef CHARACTER_INPUT_DEVICE_H
#define CHARACTER_INPUT_DEVICE_H

namespace IO
{   
    //Baseclass for Character input devices
    class CharacterInputDevice
    {
    private:
        bool isDecChar(char c) { return c >= '0' && c <= '9'; }
        bool isHexChar(char c) { return isDecChar(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'); }
        
        int hexChar2int(char ch)
        {
            if ((ch >= 'a') && (ch <= 'f')) return (ch-'a'+10);
            if ((ch >= '0') && (ch <= '9')) return (ch-'0');
            if ((ch >= 'A') && (ch <= 'F')) return (ch-'A'+10);
            return -1;
        }
        
        int char2int(char ch)
        {
            if ((ch >= '0') && (ch <= '9')) return (ch-'0');
            return -1;
        }

    protected:
		CharacterInputDevice() {}
        virtual ~CharacterInputDevice() {}
        
        enum InputType
        {
            DecChar=0,
            HexChar=1,
            ASCIIChar=2
        };
        bool IsTypeOfChar(InputType t, char c);
        
        //This method returns true if the next char equals the given char
        //The current char is not discarded and can be fetched with GetCharFromDevice
        virtual bool IsNextChar(char c) = 0;
        
        //This method return either a char if it matches the given input type
        //or NULL if the next char would be of another type
        //If NULL is returned the next char is not discarded
        virtual char GetCharFromDevice(InputType type) = 0;

    public:
        void ReadLine(char *line, int maxLen, char end = '\n');
		int ReadHex();
        unsigned int ReadUnsignedDec();
        int ReadDec();
        char ReadChar();
        
		/*CharacterOutputDevice &operator<<(char *c);
        CharacterOutputDevice &operator<<(const char *c);
        CharacterOutputDevice &operator<<(unsigned int i);
        CharacterOutputDevice &operator<<(int i);*/
    };
}

#endif
