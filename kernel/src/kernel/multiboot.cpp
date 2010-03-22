#include <kernel/global.h>
#include <kernel/multiboot.h>

using namespace Kernel;

//using namespace IO;

Multiboot::Multiboot(MultibootHeader *h)
{
    header = h;
	
    if(!IsElf()) PANIC("Kernel is not ELF-compatible!");
    elfInfo = new ElfInformation(header->elf_addr, header->elf_shndx, header->elf_size, header->elf_num);
}
