#include <arch/i386/memory.h>
#include <arch/i386/bioscall.h>
#include <memory.h>
#include <lib.h>
#include <print.h>

extern int end; //defined in linker script

FirmwareMemoryMapItem FirmwareMemoryMap[MAX_MEMORY_MAP_ENTRIES] = {
    {MemoryTypeLoaderTemporary, 0x0,                                                            0x1},       //Realmode interrupt vectors
    {MemoryTypeLoaderTemporary, BIOSCALLBUFFER/ARCH_PAGE_SIZE,                                  0x6},       //Real mode result buffer
    {MemoryTypeLoaderStack, (GXLDR_RMODE_STACKTOP-GXLDR_RMODE_STACKSIZE)/ARCH_PAGE_SIZE,        0x1},       //Real mode stack
    {MemoryTypeLoaderStack, (GXLDR_STACKTOP-GXLDR_STACKSIZE)/ARCH_PAGE_SIZE,                    0x1},       //Protected mode stack
};

void mem_i386_build_memory_map()
{
    //Insert executable location
    FirmwareMemoryMap[4].Type = MemoryTypeLoaderExecutable;
    FirmwareMemoryMap[4].BasePage = GXLDR_EXEC_BASE/ARCH_PAGE_SIZE;
    FirmwareMemoryMap[4].PageCount = ((size_t)&end)/ARCH_PAGE_SIZE;         //TODO: page align?

    //NULL out the rest
    memset(&FirmwareMemoryMap[5], 0, (MAX_MEMORY_MAP_ENTRIES-5) * sizeof(FirmwareMemoryMapItem));
    
    //TODO: Fallback to other methods for obtaining bios memory map
    mem_i386_e820_detect();
}

void mem_i386_e820_detect()
{
    uint32_t i = 0;
    
    BiosMemoryMap map;
    
    bios_int_registers regs;
    memset(&regs, 0, sizeof(bios_int_registers));
    
    while(true) {
        regs.eax = 0x0000E820;
        regs.edx = 0x534D4150;
        regs.ecx = sizeof(BiosMemoryMap);
        
        regs.es = BIOSCALLBUF_SEGMENT;
        regs.edi = BIOSCALLBUF_OFFSET;
        
        callInt(0x15, &regs);
        
        if(regs.eax != 0x534D4150) {    //TODO: check carry flag for success
            debug_printf("Bios call 0x15 failed\r\n");
            return;
        }
        
        memcpy(&map, (void*)BIOSCALLBUFFER, regs.ecx);
        
        debug_printf("Got BIOS descriptor: Base: %X - Lenght: %X - Type: %x\n", map.BaseAddress, map.Length, map.Type);
        
        //convert bios entry to loader memory map entry
        if(map.Type == BiosMemoryUsable)
        {
            MemoryType t = MemoryTypeFree;
            // uint64_t basePage = map.BaseAddress & ~(ARCH_PAGE_SIZE - 1ULL);
            // uint64_t size = map.BaseAddress + map.Length - basePage;
            // size &= ~(ARCH_PAGE_SIZE - 1ULL);
            
            uint64_t basePage = PAGE_START(map.BaseAddress) / ARCH_PAGE_SIZE;
            uint64_t size = PAGE_END(map.Length) / ARCH_PAGE_SIZE;

            memory_add_map_entry(FirmwareMemoryMap, MAX_MEMORY_MAP_ENTRIES, basePage, size, t);
        }
        else
        {
            MemoryType t = MemoryTypeSpecial;
            if(map.Type == BiosMemoryBad)
                t = MemoryTypeBad;
            
            uint64_t basePage = PAGE_START(map.BaseAddress) / ARCH_PAGE_SIZE;
            uint64_t size = PAGE_END(map.Length) / ARCH_PAGE_SIZE;

            memory_add_map_entry(FirmwareMemoryMap, MAX_MEMORY_MAP_ENTRIES, basePage, size, t);
        }

        if(regs.ebx == 0)
            return;
        
        i++;
    }
}

