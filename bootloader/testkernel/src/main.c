#include <testkernellib/lib.h>
#include <testkernel/kernelapi.h>

typedef struct _MEMORY_DESCRIPTOR
{
    unsigned int start;
    unsigned int length;
    unsigned int type;
} MEMORY_DESCRIPTOR, *PMEMORY_DESCRIPTOR;

typedef struct _LDRBLK_LOADED_IMAGE {
    char Name[32];
    unsigned int PhysicalBase;
    unsigned int VirtualBase;
    unsigned int VirtualEntryPoint;
    unsigned int SizeOfImage;
    //int ImageType;
} LOADED_IMAGE, *PLOADED_IMAGE;

typedef struct _LOADER_BLOCK
{
    unsigned int LoaderBlockSize;
    unsigned short LoadedImageCount;
    LOADED_IMAGE LoadedImages[32];
    unsigned short MemoryDescriptorCount;
    MEMORY_DESCRIPTOR MemoryDescriptors[32];
    unsigned int ACPIRDSPDescriptor;
    unsigned int UpperMemoryBoundary;
    //TODO: more?
} LOADER_BLOCK, *PLOADER_BLOCK;

void constr(void) __attribute__((constructor));
int kmain(PLOADER_BLOCK loaderBlock);
void shittyitoa(unsigned int val, char* buf);

char bigArray[1024*1024];
int globalInt = 0;

void constr()
{
    globalInt = 42;
}

int kmain(PLOADER_BLOCK loaderBlock)
{
    unsigned char *videoram = (unsigned char *) 0xb8000;
    
    for(int i = 0; i < 25*80*2; i++)
        videoram[i] = 0;

    int c = 0;

    // char number[20] = {0};
    // shittyitoa((unsigned int)(&loaderBlock->LoadedImages[0].Name), number);
    // int j = 0;
    // while(number[j])
    // {
    //     videoram[c] = number[j];
    //     videoram[c+1] = 0x07;

    //     j++;
    //     c+=2;
    // }

    c = 0;
    const char* name = loaderBlock->LoadedImages[0].Name;
    while(*name)
    {
        videoram[c] = *name++;
        videoram[c+1] = 0x07;

        c+=2;
    }
    

    for(unsigned int i = 0; i < sizeof(bigArray); i++)
    {
    	//bigArray[i] = libfunc(i);
        bigArray[i] = 0;
    }
    
    return 0;
}

const char numchars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
void shittyitoa(unsigned int val, char* buf)
{
    unsigned int mask = 0xF0000000;
    unsigned int shift = 28;

    int pos = 0;
    while(pos < 8)
    {
        buf[pos] = numchars[(val & mask) >> shift];

        pos++;
        mask >>= 4;
        shift -= 4;
    }

    buf[pos] = 0;
}

int kernelapifunc(int a)
{
    return globalInt++;
}
