#include <halinterface/BaseDebugOutputDevice.h>
#include <string.h>

using namespace Debug;

void BaseDebugOutputDevice::PrintString(char *c)
{
    unsigned int i = 0;
    while(c[i])
    {
        PrintChar(c[i]);
        i++;
    }
}

void BaseDebugOutputDevice::PrintHex(unsigned int n)
{
    char hexChars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    
    char num[20];
    memset(num, 0, 20);
    
    unsigned int i = 0;
    do
    {
        num[i] = hexChars[n % 16];
        n /= 16;
        
        i++;
    } while(i < sizeof(unsigned int)*2);
    
    reverseArray(num);
    PrintString("0x");
    PrintString(num);
}

void BaseDebugOutputDevice::PrintDec(unsigned int n)
{
    char num[20];
    memset(num, 0, 20);
    
    int i = 0;
    do
    {
        num[i] = (char)((n % 10) + '0');
        n /= 10;
        
        i++;
    } while(n != 0);
    
    reverseArray(num);
    PrintString(num);
}

void BaseDebugOutputDevice::PrintDec(int n)
{
    int sign;
    char num[20];
    memset(num, 0, 21);
    
    if((sign = n) < 0) n = -n;
    
    int i = 0;
    do
    {
        num[i] = (char)((n % 10) + '0');
        n /= 10;
        
        i++;
    } while(n != 0);
    
    if(sign < 0)
        num[i] = '-';
    
    reverseArray(num);
    PrintString(num);
}

void BaseDebugOutputDevice::PrintData(char* start, size_t len)
{
    for(unsigned int i = 0; i < (len / 0x10)+1; i++)
    {
        PrintHex((unsigned)start);
        PrintString(": ");
        
        char* ptr = (char*)start;
        for(int j = 0; j < 0x10; j++)
        {
            PrintHex(*ptr++);
            PrintString(" ");
        }
        
        ptr = (char*)start;
        PrintString("\t");
        for(int j = 0; j < 0x10; j++)
        {
            char c = *ptr++;
            if(c < 0x20 || c > 0x7e)
                PrintChar('.');
            else
                PrintChar(c);
        }
        
        PrintString("\n");
        start += 0x10;
    }
}

BaseDebugOutputDevice &BaseDebugOutputDevice::operator<<(char *c)
{
    PrintString(c);
    return *this;
}

BaseDebugOutputDevice &BaseDebugOutputDevice::operator<<(const char *c)
{
    PrintString(c);
    return *this;
}

BaseDebugOutputDevice &BaseDebugOutputDevice::operator<<(unsigned int i)
{
    if (printMode == dec)
        PrintDec(i);
    else
        PrintHex(i);
    
    return *this;
}

BaseDebugOutputDevice &BaseDebugOutputDevice::operator<<(int i)
{
    if (printMode == dec)
        PrintDec(i);
    else
        PrintHex(i);
    
    return *this;
}

BaseDebugOutputDevice &BaseDebugOutputDevice::operator<<(Special s)
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

void BaseDebugOutputDevice::reverseArray(char* arr)
{
    int i, j;
    for (i = 0, j = strlen(arr)-1; i<j; i++, j--)
    {
        char c = arr[i];
        arr[i] = arr[j];
        arr[j] = c;
    }
}
