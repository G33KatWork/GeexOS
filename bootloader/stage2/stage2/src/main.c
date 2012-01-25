#include <types.h>
#include <arch.h>
#include <print.h>

struct vbeControllerInfo {
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
} __attribute__ ((packed));

int kmain(void);

#include <arch/i386/bioscall.h> //just for testing

int kmain()
{
    arch_machine_setup();
    printf("GXLDR Stage 2\r\n");
    
    //just for testing realmode stuff
    struct vbeControllerInfo* info = (struct vbeControllerInfo*)REALMODE_PTR(0x2000, 0);
    memcpy(info->signature, "VBE2", 4);   //Tell the VGABIOS we understand VBE2
    
    bios_int_registers bregs;
    memset(&bregs, 0, sizeof(bios_int_registers));
    
    bregs.eax = 0x4F00;
    bregs.es = 0x2000;
    bregs.edi = 0;
    printf("doing int 0x10 VBE\r\n");
    callInt(0x10, &bregs);
    printf("int 0x10 done\r\n");
    
    if(bregs.eax == 0x4f)
        printf("Yay! worked! oem string: %s", (char*)REALMODE_PTR(info->oemString[1],info->oemString[0]));
    else
        printf("Nope :(");
    
    
    /*int a = 1;
    int b = 2;
    
    int c = a/(b-2);*/

    return 0;
}
