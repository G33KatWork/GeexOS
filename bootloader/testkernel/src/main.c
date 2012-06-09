#include <testkernellib/lib.h>

int kmain(void);

char bigArray[1024*1024];

int kmain()
{
    unsigned char *videoram = (unsigned char *) 0xb8000;
    
    for(int i = 0; i < 25*80*2; i++)
        videoram[i] = 0;

    videoram[0] = 65;
    videoram[1] = 0x07;

    for(int i = 0; i < sizeof(bigArray); i++)
    {
    	bigArray[i] = libfunc(i);
    }
    
    return 0;
}


