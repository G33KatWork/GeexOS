#include <types.h>
#include <arch.h>
#include <print.h>
#include <memory.h>
#include <disk.h>
#include <fs.h>
#include <heap.h>
#include <loaders/geexos.h>

int kmain(void);

//extern int start;
//extern int end;

extern Heap* default_heap;
int kmain()
{
    arch_early_machine_setup();
    debug_init();

    //change the ports if necessary
    //debug_enableOutputPort(DBGPORT_SCREEN);
    //debug_disableOutputPort(DBGPORT_RS232);
    //debug_setMessageComPort(DBGPORT_RS232_COM2);
    //debug_setGdbComPort(DBGPORT_RS232_COM1);

    //Uncomment for gdb remote debugging over serial port
    //arch_gdbstub_enable();

    arch_machine_setup();
    
    printf("GXLDR Stage 2\r\n");
    
    debug_printf("Firmware memory map:\r\n");
    memory_print_map(FirmwareMemoryMap);

    memory_init();
    default_heap_init();
    disk_init();
    fs_init();

    //TODO: parse a config here or drop to a shell

    loader_loadGeexOS();

    //memory_print_alloc_map();

    fs_shutdown();
    disk_destroy();

    heap_printFreelist(default_heap);

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
