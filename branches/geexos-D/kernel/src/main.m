//#import "TestClass.h"
#include <string.h>

int gp;

int main(void)
{   
    char* video = (char*)0xC00B8000;
    for(int i = 0; i < 24*80*2; i++)
        video[i] = 0;
 
    //TestClass* c = [TestClass init];
    
    const char src[] = "Hallo";
    char dst[6];
    
    memcpy(dst, src, 6);
    
    char* txt = dst;
    int i = 0;
    while(*txt)
    {
        video[i] = *txt;
        i++;
        video[i] = 0x03;
        i++;
        txt++;
    }
        
    return 0; 
}
