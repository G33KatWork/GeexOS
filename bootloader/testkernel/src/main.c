int kmain(void);

int kmain()
{
    unsigned char *videoram = (unsigned char *) 0xb8000;
    
    for(int i = 0; i < 25*80*2; i++)
        videoram[i] = 0;

    videoram[0] = 65;
    videoram[1] = 0x07;
    
    return 0;
}
