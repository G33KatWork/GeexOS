#include <testkernellib/lib.h>


typedef struct _MEMORY_DESCRIPTOR
{
    int start;
    int length;
    int type;
} MEMORY_DESCRIPTOR, *PMEMORY_DESCRIPTOR;

typedef struct _LDRBLK_LOADED_IMAGE {
    char    Name[32];
    int PhysicalBase;
    int VirtualBase;
    int VirtualEntryPoint;
    int  SizeOfImage;
} LOADED_IMAGE, *PLOADED_IMAGE;

typedef struct _LOADER_BLOCK
{
    LOADED_IMAGE LoadedImages[32];
    MEMORY_DESCRIPTOR MemoryDescriptors[32];
    int ACPIRDSPDescriptor;
    //TODO: more?
} LOADER_BLOCK, *PLOADER_BLOCK;


int kmain(PLOADER_BLOCK loaderBlock);

char bigArray[1024*1024];

int kmain(PLOADER_BLOCK loaderBlock)
{
    unsigned char *videoram = (unsigned char *) 0xb8000;
    
    for(int i = 0; i < 25*80*2; i++)
        videoram[i] = 0;

    int c = 0;
    const char* name = loaderBlock->LoadedImages[0].Name;
    while(*name)
    {
        videoram[c] = *name++;
        videoram[c+1] = 0x07;

        c+=2;
    }
    

    for(unsigned int i = 0; i < sizeof(bigArray); i++)
    {
    	bigArray[i] = libfunc(i);
        //bigArray[i] = 0;
    }
    
    return 0;
}


