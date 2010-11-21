#include <stdlib.h>
#include <string.h>

static char hexChars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

static void reverseArray(char* arr)
{
    int i, j;
    for (i = 0, j = strlen(arr)-1; i<j; i++, j--)
    {
        char c = arr[i];
        arr[i] = arr[j];
        arr[j] = c;
    }
}

int abs(int i)
{
    if(i < 0)
        return -i;
    return i;
}

char* itoa(int value, char * str, int base)
{
    int sign;
    
    if((sign = value) < 0) value = -value;
    
    int i = 0;
    do
    {
        str[i] = hexChars[value % base];
        value /= base;
        
        i++;
    } while(value != 0);
    
    if(sign < 0)
        str[i] = '-';
    
    str[i+1] = 0;
    reverseArray(str);
    str[i+1] = 0;
    return str;
}
