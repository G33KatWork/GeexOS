#include <MultibootInformation.h>
#include <print.h>
#include <paging.h>
#include <lib.h>
#include <elf.h>
#include <KernelInfo.h>

//Physical start address for kernel and module image
//FIXME: while mapping, the biosregions should be checked. We don't want to overwrite important stuff!
#define     IMAGE_MAPPING_PHYSICAL_START        0x200000

Address currentPhysicalMapAddress = IMAGE_MAPPING_PHYSICAL_START;
KernelInformation kernelInformation;
KernelInformationProgramRegion programRegions[32];

typedef void (*elfEntry)(KernelInformation*);

int kmain(MultibootInfo* multibootInformation, unsigned int magic);
uint32_t mapElfFile(void* elfPointer);

int kmain(MultibootInfo* multibootInformation, unsigned int magic)
{
    if(magic != 0x2BADB002)
        panic("ERROR: Multiboot magic value doesn't match. Can't assume that multiboot information is correct. Halting.");
    
    print_clear();
    print_string_static("Loaderstub running.\r\n");
    
    print_string_static("Building paging structures...\r\n");
    paging_init();
    print_string_static("Enabling paging...\r\n");
    paging_set_page_directory(&pageDirectory);
	print_string_static("Paging enabled.\r\n");
	
    print_string_static("Got "); print_integer(multibootInformation->modsCount); print_string_static(" modules from GRUB\r\n");
    
    if(multibootInformation->modsCount != 2)
        panic("ERROR: Wrong number of modules provided. It need to be exactly 2: The kernel and the module package. Halting.");
    
    multiboot_module_region_t module = multibootInformation->modules[0];
    if(!elf32_isElf((void*)module.modStart))
        panic("ERROR: First supplied module is not an ELF file.");
    
    uint32_t numProgramSections = mapElfFile((void*)module.modStart);
    
    //retrieve symbol and string table from ELF
    size_t symtabSize;
    Address symtabAddress = elf32_getSectionAddress((void*)module.modStart, ".symtab", &symtabSize);
    print_string_static("Symbol table is at "); print_integer_hex(symtabAddress); print_string_static(" with size "); print_integer_hex(symtabSize); print_string_static("\r\n");
    
    size_t strtabSize;
    Address strtabAddress = elf32_getSectionAddress((void*)module.modStart, ".strtab", &strtabSize);
    print_string_static("String table is at "); print_integer_hex(strtabAddress); print_string_static(" with size "); print_integer_hex(strtabSize); print_string_static("\r\n");
    
    //build kernel environment information
    //this is all inside the first 4MB which are still identity mapped when jumping into the kernel
    //the kernel is responsible to retrieve this information and copy it into a safe place before
    //unmapping this first 4 MB. Since the kernel will, most likely, initialize the paging as a first
    //subsystem it's OK.
    kernelInformation.memoryLo = multibootInformation->memoryLo;
    kernelInformation.memoryHi = multibootInformation->memoryHi;
    kernelInformation.cmdLine = module.cmdLine;
    kernelInformation.moduleStartAddress = multibootInformation->modules[1].modStart;
    kernelInformation.moduleLength = multibootInformation->modules[1].modEnd - multibootInformation->modules[1].modStart;
    kernelInformation.memoryRegionCount = multibootInformation->mmap_length / sizeof(KernelInformationMemoryRegion);
    kernelInformation.memoryRegions = (KernelInformationMemoryRegion*)multibootInformation->mmap_addr;
    kernelInformation.programRegionCount = numProgramSections;
    kernelInformation.programRegions = programRegions;
    kernelInformation.symtabAddr = symtabAddress;
    kernelInformation.symtabSize = symtabSize;
    kernelInformation.strtabAddr = strtabAddress;
    kernelInformation.strtabSize = strtabSize;
    
    elfEntry entrypoint = (elfEntry)elf32_getEntryPoint((void*)module.modStart);
    print_string_static("Jumping to entrypoint "); print_integer_hex((Address)entrypoint); print_string_static("\r\n");
    entrypoint(&kernelInformation);
    
    return 0;
}

uint32_t mapElfFile(void* elfPointer)
{
    Elf32ProgramHeader* header;
    int numHeaders = elf32_getProgramHeaders(elfPointer, &header);
    print_string_static("Found "); print_integer(numHeaders); print_string_static(" program headers\r\n");
    
    if(numHeaders == 0)
        panic("No program headers in ELF file found. WTF?");
    
    if(numHeaders >= 32)
        panic("Too many program headers.");
    
    for(int i = 0; i < numHeaders; i++)
    {
        if(header->type != ELF_PROGRAM_HEADER_TYPE_LOAD)
            panic("ERROR: Don't know how to handle ELF program header which is not of type LOAD");
        
        print_string_static("Found program header of type "); print_integer(header->type);
        print_string_static(" at offset "); print_integer_hex(header->offset);
        print_string_static(" for virt "); print_integer_hex(header->vaddr);
        print_string_static(" with size "); print_integer_hex(header->memsz);
        print_string_static(" and flags "); print_integer_hex(header->flags); print_string_static("\r\n");
        
        //page align size if necessary    
        uint32_t sectionSize = IS_PAGE_ALIGNED(header->memsz) ? header->memsz : PAGE_ALIGN(header->memsz);
        for(Address a = header->vaddr; a < header->vaddr + sectionSize; a += PAGE_SIZE)
        {
            //print_string_static("Mapping "); print_integer_hex(a); print_string_static("\r\n");
            paging_map(currentPhysicalMapAddress, a, TRUE, FALSE, FALSE);
            currentPhysicalMapAddress += PAGE_SIZE;
        }
        
        //zero memory
        memset((void*)header->vaddr, 0, sectionSize);
        
        //if there is data for this section, copy it
        if(header->filesz > 0)
        {
            print_string_static("Copying from "); print_integer_hex((Address)elfPointer + header->offset); print_string_static(" to "); print_integer_hex(header->vaddr); print_string_static("\r\n");
            memcpy((void*)header->vaddr, (void*)((Address)elfPointer + header->offset), sectionSize);
        }
        else
            print_string_static("No data for this section\r\n");
        
        //set flags as defined for this section
        for(Address a = header->vaddr; a < header->vaddr + sectionSize; a += PAGE_SIZE)
            paging_setflags(a, header->flags & ELF_PROGRAM_HEADER_FLAG_WRITE, FALSE, header->flags & ELF_PROGRAM_HEADER_FLAG_EXEC);
        
        //fill programRegions with section
        programRegions[i].vaddr = header->vaddr;
        programRegions[i].len = sectionSize;
        programRegions[i].flags = header->flags;
        
        header++;
    }
    
    return numHeaders;
}
