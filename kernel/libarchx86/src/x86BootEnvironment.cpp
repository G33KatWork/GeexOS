#include <arch/x86BootEnvironment.h>
#include <string.h>
#include <kernel/global.h>

using namespace Arch;

x86BootEnvironment::x86BootEnvironment(MultibootInfo* i)
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
