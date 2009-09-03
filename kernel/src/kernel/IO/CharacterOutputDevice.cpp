#include <kernel/IO/CharacterOutputDevice.h>
#include <lib/string.h>

using namespace IO;

CharacterOutputDevice::CharacterOutputDevice()
{
    printMode = dec;
    
    foregroundColor = White;
    backgroundColor = Black;
}

void CharacterOutputDevice::PrintString(char *c)
{
    unsigned int i = 0;
    while(c[i])
    {
        PrintChar(c[i]);
        i++;
    }
}

void CharacterOutputDevice::PrintHex(unsigned int n)
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

void CharacterOutputDevice::PrintDec(unsigned int n)
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

void CharacterOutputDevice::PrintDec(int n)
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

void CharacterOutputDevice::Clear(Color c)
{
    backgroundColor = c;
    Clear();
}

CharacterOutputDevice &CharacterOutputDevice::operator<<(char *c)
{
    PrintString(c);
    return *this;
}

CharacterOutputDevice &CharacterOutputDevice::operator<<(const char *c)
{
    PrintString(c);
    return *this;
}

CharacterOutputDevice &CharacterOutputDevice::operator<<(unsigned int i)
{
    if (printMode == dec)
        PrintDec(i);
    else
        PrintHex(i);
    
    return *this;
}

CharacterOutputDevice &CharacterOutputDevice::operator<<(int i)
{
    if (printMode == dec)
        PrintDec(i);
    else
        PrintHex(i);
    
    return *this;
}

CharacterOutputDevice &CharacterOutputDevice::operator<<(Special s)
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

void CharacterOutputDevice::reverseArray(char* arr)
{
    int i, j;
    for (i = 0, j = strlen(arr)-1; i<j; i++, j--)
    {
        int c = arr[i];
        arr[i] = arr[j];
        arr[j] = c;
    }
}
