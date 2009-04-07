module kernel.main;

extern(C) void main(uint magic, uint addr)
{
        byte *videoram = cast( byte * ) 0xb8000; // Set up video memory
        videoram[0] = 'D';
        videoram[1] = 0x07;
        
        //clear the rest
        for(int i = 0; i < 80*24*2 - 2; i++)
            videoram[i+2] = 0;
}
