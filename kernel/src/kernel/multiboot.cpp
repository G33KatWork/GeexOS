#include <kernel/global.h>
#include <kernel/multiboot.h>

using namespace Kernel;

#define ALIGN_UP(addr, align) \
	((addr + (typeof (addr)) align - 1) & ~((typeof (addr)) align - 1))

Multiboot::Multiboot(MultibootInfo *i)
{
    info = i;
	
    if(!IsElf()) PANIC("Kernel is not ELF-compatible!");
    elfInfo = new ElfInformation(info->elf_addr, info->elf_shndx, info->elf_size, info->elf_num);
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
