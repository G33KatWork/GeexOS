#include <halinterface/BaseOutputDevice.h>
#include <string.h>

using namespace Debug;

void BaseOutputDevice::PrintString(char *c)
{
    unsigned int i = 0;
    while(c[i])
    {
        PrintChar(c[i]);
        i++;
    }
}

void BaseOutputDevice::PrintHex(uint64_t n)
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
    } while(n != 0);
    
    reverseArray(num);
    PrintString("0x");
    PrintString(num);
}

void BaseOutputDevice::PrintDec(uint64_t n)
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

void BaseOutputDevice::PrintDec(int64_t n)
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

void BaseOutputDevice::PrintData(char* start, size_t len)
{
    for(unsigned int i = 0; i < (len / 0x10)+1; i++)
    {
        PrintHex((uint64_t)start);
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
        
        PrintString("\r\n");
        start += 0x10;
    }
}

BaseOutputDevice &BaseOutputDevice::operator<<(char *c)
{
    PrintString(c);
    return *this;
}

BaseOutputDevice &BaseOutputDevice::operator<<(const char *c)
{
    PrintString(c);
    return *this;
}

BaseOutputDevice &BaseOutputDevice::operator<<(unsigned int i)
{
    if (printMode == dec)
        PrintDec((uint64_t)i);
    else
        PrintHex(i);
    
    return *this;
}

/*BaseOutputDevice &BaseOutputDevice::operator<<(int i)
{
    if (printMode == dec)
        PrintDec(i);
    else
        PrintHex(i);
    
    return *this;
}*/

BaseOutputDevice &BaseOutputDevice::operator<<(Special s)
{
    switch(s)
    {
        case endl:
            PrintChar('\r');
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

void BaseOutputDevice::reverseArray(char* arr)
{
    int i, j;
    for (i = 0, j = strlen(arr)-1; i<j; i++, j--)
    {
        char c = arr[i];
        arr[i] = arr[j];
        arr[j] = c;
    }
}
