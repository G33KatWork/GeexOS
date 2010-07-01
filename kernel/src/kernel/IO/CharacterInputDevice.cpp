#include <kernel/IO/CharacterInputDevice.h>

using namespace IO;

bool CharacterInputDevice::IsTypeOfChar(CharacterInputDevice::InputType t, char c)
{
    switch(t)
    {
        case DecChar:
            return isDecChar(c);
        case HexChar:
            return isHexChar(c);
        default:
            return true;
    }
}

void CharacterInputDevice::ReadLine(char *line, int maxLen, char end)
{
    char c = ReadChar();
    int curLen = 0;
    while(c != end && curLen < maxLen)
    {
        line[curLen] = c;
        
        c = ReadChar();
        curLen++;
    }
}

int CharacterInputDevice::ReadHex()
{
    int num = 0;
    bool negative = false;
    
    if(IsNextChar('-'))
    {
        negative = true;
        GetCharFromDevice(ASCIIChar);
    }
    
    for(int len = 0; len < 8; len++)
    {
        char c = GetCharFromDevice(HexChar);
        if(c == NULL)
            break;
        else
            num = (num << 4) | hexChar2int(c);
    }
    
    if(negative)
        num = -num;
    
    return num;
}

unsigned int CharacterInputDevice::ReadUnsignedDec()
{
    unsigned int num = 0;
    for(;;)
    {
        char c = GetCharFromDevice(DecChar);
        if(c == NULL)
            break;
        else
            num = num * 10 + char2int(c);
    }
    
    return num;
}

int CharacterInputDevice::ReadDec()
{
    int num = 0;
    bool negative = false;
    
    if(IsNextChar('-'))
    {
        negative = true;
        GetCharFromDevice(ASCIIChar);
    }
    
    for(;;)
    {
        char c = GetCharFromDevice(DecChar);
        if(c == NULL)
            break;
        else
            num = num * 10 + char2int(c);
    }
    
    if(negative)
        num = -num;
    
    return num;
}

char CharacterInputDevice::ReadChar()
{
    return GetCharFromDevice(ASCIIChar);
}

/*CharacterOutputDevice &operator<<(char *c);
CharacterOutputDevice &operator<<(const char *c);
CharacterOutputDevice &operator<<(unsigned int i);
CharacterOutputDevice &operator<<(int i);
CharacterOutputDevice &operator<<(size_t i);*/
