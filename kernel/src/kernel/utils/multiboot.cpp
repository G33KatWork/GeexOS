#include <kernel/global.h>
#include <kernel/utils/multiboot.h>
#include <lib/string.h>

using namespace Kernel;

#define ALIGN_UP(addr, align) \
	((addr + (typeof (addr)) align - 1) & ~((typeof (addr)) align - 1))

Multiboot::Multiboot(MultibootInfo *i)
{
    //Copy main multiboot struct
    memcpy(&info, i, sizeof(MultibootInfo));
    
    //Copy cmdline
    cmdLine = (char*)kmalloc(strlen(i->cmdLine) + 1);
    memcpy(cmdLine, i->cmdLine, strlen(i->cmdLine) + 1);
    info.cmdLine = cmdLine;
    
    //Copy memmap
    memregions = (multiboot_memory_region_t*)kmalloc(i->mmap_length);
    for(unsigned int j = 0; j < info.mmap_length / sizeof(multiboot_memory_region_t); j++)
    {
        memregions[j].size = i->mmap_addr[j].size;
        memregions[j].addr = i->mmap_addr[j].addr;
        memregions[j].len = i->mmap_addr[j].len;
        memregions[j].type = i->mmap_addr[j].type;
    }
    
    //TODO: Copy all the tables and correct addresses. For now we are only doing this with the cmdline and memory map
    
    if(!IsElf()) PANIC("Kernel is not ELF-compatible!");
}

/*size_t Multiboot::GetSize()
{
    //struct itself
    size_t size = sizeof(struct multiboot_info);
    
    //commandline
    if(info->flags & MULTIBOOT_FLAG_CMDLINE)
        size += ALIGN_UP(strlen((char*)info->cmdLine), 4);
    
    //modules + commandline each
    if(info->flags & MULTIBOOT_FLAG_MODS)
    {
        size += info->modsCount * sizeof(multiboot_module_region_t);
        size_t mod_cmdLine_size = 0;
        for(size_t i = 0; i < info->modsCount; ++i)
            mod_cmdLine_size += ALIGN_UP(strlen((char*)info->modsAddr[i].cmdLine), 4);
        size += mod_cmdLine_size;
    }
    
    //memory regions
    if(info->flags & MULTIBOOT_FLAG_MMAP)
        size += info->mmap_length * sizeof(multiboot_memory_region_t);
    
    //drives
    if(info->flags & MULTIBOOT_FLAG_DRIVES)
    {
        size_t drives_length = 0;
        for(size_t i = 0; i < info->drives_length; ++i)
            drives_length += info->drives_addr[i].size;    //TODO: ALIGN_UP by 4?
        size += drives_length;
    }
    
    //TODO: config_table??
    
    //bootloader name
    if(info->flags & MULTIBOOT_FLAG_LOADER)
        size += ALIGN_UP(strlen((char*)info->bootloader_name), 4);
        
    //apm table
    if(info->flags & MULTIBOOT_FLAG_APM)
        size += sizeof(multiboot_apm_region_t);
        
    //TODO: VBE table???
    
    return size;
}*/
