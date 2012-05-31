#include <types.h>
#include <arch.h>
#include <print.h>
#include <memory.h>
#include <disk.h>
#include <fs.h>

int kmain(void);

//extern int start;
//extern int end;

int kmain()
{
    arch_machine_setup();
    printf("GXLDR Stage 2\r\n");
    
    //memory_print_map(FirmwareMemoryMap);
    
    memory_init();
    default_heap_init();
    disk_init();
    fs_init();

    disk_printdevices();

    FILE* file = open("hd(0,0)/GXLDR");
    if(file)
    {
      printf("File seems successfully opened...\n");
      

      printf("Reading 0x10000 bytes...\n");
      uint8_t* buffer = malloc(0x100);
      size_t r = read(file, buffer, 0x100);
      printf("Read returned %x\n", r);
      printf("First few bytes: %x %x %x %x\n", buffer[0], buffer[1], buffer[2], buffer[3]);

      seek(file, -1000, SEEK_END);

      r = read(file, buffer, 0x100);
      printf("Read returned %x\n", r);
      printf("First few bytes: %x %x %x %x\n", buffer[0], buffer[1], buffer[2], buffer[3]);
    }
      

    

    /*FILE* file2 = open("hd(0,0)/SOMEDIR/AFILE.TXT");
    if(file2)
      printf("File 2 seems successfully opened...\n");
    else
      printf("file2? Nope...\n");

    FILE* file3 = open("hd(0,0)/FILE.FOO");
    if(file3)
      printf("File 3 seems successfully opened...\n");
    else
      printf("file3? Nope...\n");

    FILE* file4 = open("hd(0,0)/BaR");
    if(file4)
      printf("File 4 seems successfully opened...\n");
    else
      printf("file4? Nope...\n");*/

    //close(file);
    //close(file2);

    //memory_print_alloc_map();
    
    /*DiskGeometry g;
    if(biosdisk_i386_get_disk_geometry(0, &g))
    {
        printf("BIOS disk read succeeded :)\n");
        printf("%x Cylinders\n%x Heads\n%x Sectors\n%x BytesPerSector\n", g.Cylinders, g.Heads, g.Sectors, g.BytesPerSector);
    }
    else
        printf("BIOS disk geometry get failed :(\n");*/

    /*if(biosdisk_i386_get_disk_geometry(0x80, &g))
    {
        printf("BIOS disk read succeeded :)\n");
        printf("%x Cylinders\n%x Heads\n%x Sectors\n%x BytesPerSector\n", g.Cylinders, g.Heads, g.Sectors, g.BytesPerSector);
    }
    else
        printf("BIOS disk geometry get failed :(\n");*/

    /*#define NUMBLOCKS 6
    uint8_t buffer[NUMBLOCKS * 512];
    //uint8_t* buffer = malloc(1440*1024);
    if(biosdisk_i386_read_sectors(0x0, 50, NUMBLOCKS, buffer))
    {
        printf("BIOS disk read succeeded :)\n");
    }
    else
    {
        printf("BIOS disk read failed :(\n");
    }*/

    return 0;
}





//#include <arch/i386/bioscall.h> //just for testing

/*struct vbeControllerInfo {
   char     signature[4];             // == "VESA"
   uint16_t version;                 // == 0x0300 for VBE 3.0
   uint16_t oemString[2];            // isa vbeFarPtr
   uint32_t capabilities;
   uint16_t videomodes[2];           // isa vbeFarPtr
   uint16_t totalMemory;             // as # of 64KB blocks
   uint16_t oemSoftwareRev;
   uint16_t oemVendorNamePtr[2];
   uint16_t oemProductNamePtr[2];
   uint16_t oemProductRevPtr[2];
   uint8_t  reserved[222];
   uint8_t  oemData[256];
} __attribute__ ((packed));*/

//just for testing realmode stuff
/*struct vbeControllerInfo* info = (struct vbeControllerInfo*)REALMODE_PTR(BIOSCALLBUF_SEGMENT, BIOSCALLBUF_OFFSET);
memcpy(info->signature, "VBE2", 4);   //Tell the VGABIOS we understand VBE2

bios_int_registers bregs;
memset(&bregs, 0, sizeof(bios_int_registers));

bregs.eax = 0x4F00;
bregs.es = BIOSCALLBUF_SEGMENT;
bregs.edi = BIOSCALLBUF_OFFSET;
printf("doing int 0x10 VBE\r\n");
callInt(0x10, &bregs);
printf("int 0x10 done\r\n");

if(bregs.eax == 0x4f)
    printf("Yay! worked! oem string: %s", (char*)REALMODE_PTR(info->oemString[1],info->oemString[0]));
else
    printf("Nope :(");*/
