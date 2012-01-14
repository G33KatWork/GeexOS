int kmain(void)
{
    /* Print a letter to screen to see everything is working: */
    unsigned char *videoram = (unsigned char *) 0xb8000;
    
    /* Clear screen */
    for(int i = 0; i < 25*80*2; i++)
        videoram[i] = 0;

    videoram[0] = 65; /* character 'A' */
    videoram[1] = 0x07; /* forground, background color. */

    return 0;
}
