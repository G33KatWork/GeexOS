#include <exeformats/elfloader.h>
#include <fs.h>
#include <print.h>
#include <arch.h>

static bool elf_getElf32Header(FILE* imageFile, LoadedImage* imageInformation);
static bool elf_getElf32Phdrs(FILE* imageFile, LoadedImage* imageInformation);

static bool elf_loadElf32File(FILE* imageFile, MemoryType memType, LoadedImage* imageInformation);
static bool elf_linkElf32File(LoadedImage* imageInformation);
static bool elf_elf32doRelocation(LoadedImage* imageInformation, size_t relCount, Elf32_Rel* relocs);
static Elf32_Sym* elf_elf32lookupSymbol(LoadedImage* imageInformation, const char* symbol);
static Elf32_Addr elf32_getImageSize(LoadedImage* imageInformation, Elf32_Addr* minAddr, Elf32_Addr* maxAddr);

static bool elf_loadElf64File(FILE* imageFile, MemoryType memType, LoadedImage* imageInformation);

typedef struct
{
	//ELF-Header in memory
	Elf32_Ehdr* header;

	//program headers
	size_t phdrCount;
	Elf32_Phdr* phdrs;

	//PLT Relocations
	size_t pltRelCount;
	Elf32_Rel* pltRel;

	//normal relocations
	size_t relCount;
	Elf32_Rel* rel;

	//Symbol and string table
	const char* strtab;
	Elf32_Sym* symtab;

	//Hash table data
	size_t nbucket;
	size_t nchain;
	unsigned* bucket;
	unsigned* chain;
} elf32_loaderContext;

static void elf32_freeLoaderContext(elf32_loaderContext* context)
{
	free(context->header);
	free(context->phdrs);
	free(context);
}

static inline void elf32_setLoaderContext(LoadedImage* imageInformation, elf32_loaderContext* context)
{
	imageInformation->Context = context;
}

static inline elf32_loaderContext* elf32_getLoaderContext(LoadedImage* imageInformation)
{
	return (elf32_loaderContext*)imageInformation->Context;
} 

bool elf_loadFile(const char* filename, MemoryType memType, LoadedImage** imageInformation)
{
	LoadedImage* loadedImageInfo = NULL;

	FILE* imageFile = open(filename);
	if(!imageFile)
		return false;

	unsigned char e_ident[EI_NIDENT];
	if(read(imageFile, &e_ident, EI_NIDENT) != EI_NIDENT)
		goto out_error;

	if(e_ident[EI_MAG0] != ELFMAG0 || e_ident[EI_MAG1] != ELFMAG1 || e_ident[EI_MAG2] != ELFMAG2 || e_ident[EI_MAG3] != ELFMAG3)
		goto out_error;

	debug_printf("ELF: Found ELF Header magic\n");

	if(e_ident[EI_CLASS] != ELFCLASS64 && e_ident[EI_CLASS] != ELFCLASS32)
	{
		debug_printf("ELF: Invalid ELF class encountered\n");
		goto out_error;
	}

	//Create library information
	loadedImageInfo = malloc(sizeof(LoadedImage));
	if(!loadedImageInfo)
	{
		debug_printf("ELF: Error allocating loaded image info");
		goto out_error;
	}
	memset(loadedImageInfo, 0, sizeof(LoadedImage));
	strncpy(loadedImageInfo->Name, fs_getFilename(filename), 31);
	loadedImageInfo->Name[31] = 0;
	loadedImageInfo->ImageType = LOADER_IMAGETYPE_ELF;

	//Create ELF32 loader context
	elf32_loaderContext* context = malloc(sizeof(elf32_loaderContext));
	if(!context)
	{
		debug_printf("ELF: Error allocating loader context");
		goto out_error;
	}
	memset(context, 0, sizeof(elf32_loaderContext));
	elf32_setLoaderContext(loadedImageInfo, context);

	//Add library to list
	list_add(&loadedImageInfo->Link, &loader_loadedImages);

	seek(imageFile, 0, SEEK_SET);

	if(e_ident[EI_CLASS] == ELFCLASS64)
	{
		debug_printf("ELF: Trying to load an ELF64 file\n");
		if(!elf_loadElf64File(imageFile, memType, loadedImageInfo))
			goto out_error;
	}
	else
	{
		debug_printf("ELF: Trying to load an ELF32 file\n");

		if(!elf_getElf32Header(imageFile, loadedImageInfo))
			goto out_error;

		if(!elf_getElf32Phdrs(imageFile, loadedImageInfo))
			goto out_error;

		if(!elf_loadElf32File(imageFile, memType, loadedImageInfo))
			goto out_error;

		if(!elf_linkElf32File(loadedImageInfo))
			goto out_error;
	}
	
	if(imageInformation)
		*imageInformation = loadedImageInfo;

	close(imageFile);
	return true;

out_error:

	if(imageFile)
		close(imageFile);

	if(loadedImageInfo)
	{
		elf32_freeLoaderContext(elf32_getLoaderContext(loadedImageInfo));
		list_del(&loadedImageInfo->Link);
		free(loadedImageInfo);
	}

	return false;
}

static bool elf_getElf32Header(FILE* imageFile, LoadedImage* imageInformation)
{
	debug_printf("ELF32: Retrieving ELF32 header\n");

	if(!imageInformation)
		return false;

	elf32_loaderContext* context = elf32_getLoaderContext(imageInformation);
	if(!context)
	{
		debug_printf("ELF32: Loader context is NULL, whut?");
		return false;
	}

	Elf32_Ehdr* header = malloc(sizeof(Elf32_Ehdr));
	if(!header)
	{
		debug_printf("ELF32: Allocating memory for ELF32 header failed\n");
		return false;
	}

	context->header = header;

	if(read(imageFile, header, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr))
	{
		debug_printf("ELF32: Reading of ELF headers failed\n");
		return false;
	}

	if(header->e_machine != EM_386)
	{
		debug_printf("ELF32: Wrong architecture type. Expected: 0x%x, got: 0x%x\n", EM_386, header->e_machine);
		return false;
	}

	if(header->e_type != ET_DYN)
	{
		debug_printf("ELF32: Only supporting ET_DYN ELF files, got 0x%x\n", header->e_type);
		return false;
	}

	return true;
}

static bool elf_getElf32Phdrs(FILE* imageFile, LoadedImage* imageInformation)
{
	debug_printf("ELF32: Retrieving ELF32 PHDRs\n");

	if(!imageInformation)
		return false;

	elf32_loaderContext* context = elf32_getLoaderContext(imageInformation);
	if(!context)
	{
		debug_printf("ELF32: Loader context is NULL, whut?");
		return false;
	}

	if(!context->header)
	{
		debug_printf("ELF32: Pointer to ELF header in loader context is NULL\n");
		return false;
	}

	size_t phdrSize = context->header->e_phentsize * context->header->e_phnum;
	if(phdrSize > 16*1024)
	{
		debug_printf("ELF32: PHDRs are bigger than 16KByte? I don't think so...\n");
		return false;
	}

	context->phdrs = malloc(phdrSize);
	if(!context->phdrs)
	{
		debug_printf("ELF32: Couldn't allocate memory for PHDRs\n");
		return false;
	}

	context->phdrCount = context->header->e_phnum;

	Elf32_Half phoffset = context->header->e_ehsize;
	seek(imageFile, phoffset, SEEK_SET);

	if(read(imageFile, context->phdrs, phdrSize) != phdrSize)
	{
		debug_printf("ELF32: Error reading PHDRs from file\n");
		return false;
	}

	return true;
}

static bool elf_loadElf32File(FILE* imageFile, MemoryType memType, LoadedImage* imageInformation)
{
	debug_printf("ELF32: Loading ELF32 file LOAD PHDRs into memory\n");
	
	//Get total image size for all LOAD program headers
	Elf32_Addr minAddr;
	Elf32_Addr maxAddr;
	Elf32_Addr totalImageSize = elf32_getImageSize(imageInformation, &minAddr, &maxAddr);

	if(totalImageSize == 0)
	{
		debug_printf("ELF32: Failed to retrieve ELF32 image size\n");
		return false;
	}

	debug_printf("ELF32: Total image size is 0x%x\n", totalImageSize);
	debug_printf("ELF32: Minimum virtual address is 0x%x\n", minAddr);

	//TODO: Search for free memory in virtual memory space
	//TODO: ASLR
	Elf32_Addr virtualBase = minAddr;
	Elf32_Addr originalVirtualBase = virtualBase;
	if(!arch_is_virtual_memory_range_free(virtualBase, totalImageSize))
	{
		debug_printf("ELF32: virtual base address %x is already in use\n", virtualBase);
		virtualBase = arch_find_free_virtual_memory_region(virtualBase, totalImageSize);
		debug_printf("ELF32: using virtual base %x\n", virtualBase);

		if(virtualBase == 0)
		{
			debug_printf("ELF32: Can't find memory region big enough to hold ELF32 file\n");
			return false;
		}
	}

	//Allocate memory
	void* physicalBase = memory_allocate(totalImageSize, memType);
	if(!physicalBase)
	{
		debug_printf("ELF32: failed to allocate physical memory\n");
		return false;
	}

	//Map into virtual address space
	arch_map_virtual_memory_range((Address)physicalBase, virtualBase, totalImageSize, false, false);


	elf32_loaderContext* context = elf32_getLoaderContext(imageInformation);
	if(!context)
	{
		debug_printf("ELF32: Loader context is NULL, whut?");
		return false;
	}

	//Read data into memory
	for(size_t i = 0; i < context->phdrCount; i++)
	{
		Elf32_Phdr phdr = context->phdrs[i];

		if(phdr.p_type == PT_LOAD)
		{
			debug_printf("ELF32: PHDR: Vaddr: 0x%x, Offset: 0x%x, Memsize: 0x%x, Filesize: 0x%x\n", phdr.p_vaddr, phdr.p_offset, phdr.p_memsz, phdr.p_filesz);

			Address targetAddress = ((Address)physicalBase) + (phdr.p_vaddr - originalVirtualBase);

			if(phdr.p_filesz > 0)
			{
				debug_printf("ELF32: seeking to 0x%x\n", phdr.p_offset);
				seek(imageFile, phdr.p_offset, SEEK_SET);

				debug_printf("ELF32: reading 0x%x bytes to 0x%x\n", phdr.p_filesz, targetAddress);
				if(read(imageFile, (void*)targetAddress, phdr.p_filesz) != phdr.p_filesz)
				{
					debug_printf("ELF: Error while reading segment data from ELF32 file\n");
					return false;
				}
			}

			if(phdr.p_memsz > phdr.p_filesz)
			{
				debug_printf("ELF32: zeroing memory from 0x%x to 0x%x\n", targetAddress + phdr.p_filesz, targetAddress + phdr.p_filesz + (phdr.p_memsz - phdr.p_filesz));
				memset((void*)(targetAddress + phdr.p_filesz), 0, phdr.p_memsz - phdr.p_filesz);
			}
		}
	}

	Address entrypoint = (context->header->e_entry - originalVirtualBase) + virtualBase;

	imageInformation->PhysicalBase = (Address)physicalBase;
	imageInformation->VirtualBase = virtualBase;
	imageInformation->OriginalBase = originalVirtualBase;
	imageInformation->VirtualEntryPoint = entrypoint;
	imageInformation->SizeOfImage = totalImageSize;

	return true;
}

static bool elf_linkElf32File(LoadedImage* imageInformation)
{
	debug_printf("ELF32: Linking ELF32 file\n");

	elf32_loaderContext* context = elf32_getLoaderContext(imageInformation);
	if(!context)
	{
		debug_printf("ELF32: Loader context is NULL, whut?");
		return false;
	}

	for(size_t i = 0; i < context->phdrCount; i++)
	{
		Elf32_Phdr phdr = context->phdrs[i];

		if(phdr.p_type == PT_DYNAMIC)
		{
			debug_printf("ELF32: Found DYNAMIC PHDR at index %x\n", i);

			Address phdrPhysMemoryAddr = imageInformation->PhysicalBase + (phdr.p_vaddr - imageInformation->OriginalBase);
			debug_printf("ELF32: dynamic headers may be mapped at 0x%x\n", phdrPhysMemoryAddr);

			//Check wether the DYNAMIC headers are already somewhere in memory
			//TODO: do the DYNAMIC PHDRs always get loaded through a LOAD PHDR? What if not?
			if(arch_is_virtual_memory_range_free(imageInformation->VirtualBase + (phdr.p_vaddr - imageInformation->OriginalBase), phdr.p_memsz))
			{
				debug_printf("ELF32: DYNAMIC PHDRs doesn't seem to be in memory\n");
				return false;
			}

			//loop through DYNAMIC headers
			for(Elf32_Dyn* d = (Elf32_Dyn*)phdrPhysMemoryAddr; d->d_tag != DT_NULL; d++)
			{
				debug_printf("ELF32: Found DYNAMIC Tag 0x%x : 0x%x\n", d->d_tag, d->d_un.d_val);
				
				switch(d->d_tag)
				{
					case DT_PLTREL:
						if (d->d_un.d_val != DT_REL)
						{
							debug_printf("ELF32: unsupported DT_RELA\n");
							return false;
						}
						break;

					case DT_RELA:
						debug_printf("ELF32: unsupported DT_RELA\n");
						return false;

					case DT_JMPREL:
						context->pltRel = (Elf32_Rel*)(imageInformation->PhysicalBase + (d->d_un.d_ptr - imageInformation->OriginalBase));
						break;

					case DT_PLTRELSZ:
						context->pltRelCount = d->d_un.d_val / sizeof(Elf32_Rel);
						break;

					case DT_REL:
						context->rel = (Elf32_Rel*)(imageInformation->PhysicalBase + (d->d_un.d_ptr - imageInformation->OriginalBase));
						break;

					case DT_RELSZ:
						context->relCount = d->d_un.d_val / sizeof(Elf32_Rel);
						break;

					case DT_PLTGOT:
						/* Save this in case we decide to do lazy binding. We don't yet. */
						//si->plt_got = (unsigned *)(base + d->d_un.d_ptr);
						break;

					case DT_SYMTAB:
						context->symtab = (Elf32_Sym*)(imageInformation->PhysicalBase + (d->d_un.d_ptr - imageInformation->OriginalBase));
						break;

					case DT_STRTAB:
						context->strtab = (const char*)(imageInformation->PhysicalBase + (d->d_un.d_ptr - imageInformation->OriginalBase));
						break;

					case DT_HASH:
						context->nbucket = ((unsigned *) (imageInformation->PhysicalBase + (d->d_un.d_ptr - imageInformation->OriginalBase)))[0];
						context->nchain = ((unsigned *) (imageInformation->PhysicalBase + (d->d_un.d_ptr - imageInformation->OriginalBase)))[1];
						context->bucket = (unsigned *) (imageInformation->PhysicalBase + (d->d_un.d_ptr - imageInformation->OriginalBase) + 8);
						context->chain = (unsigned *) (imageInformation->PhysicalBase + (d->d_un.d_ptr - imageInformation->OriginalBase) + 8 + context->nbucket * 4);
						break;
					//TODO: INIT, FINI, PREINIT, DEBUG
				}
			}

			//TODO: check if relocations, sym/strab and hash tables are mapped into memory
			debug_printf("ELF32: Parsed DYNAMIC PHDR: 0x%x PLT relocs at 0x%x - 0x%x relocs at 0x%x - STRTAB at 0x%x - SYMTAB at 0x%x\n",
				context->pltRelCount, context->pltRel, context->relCount, context->rel, context->strtab, context->symtab);

			if(context->nbucket == 0)
			{
				debug_printf("ELF32: No symbol hash table\n");
				return false;
			}

			if(!context->strtab)
			{
				debug_printf("ELF32: STRTAB missing\n");
				return false;
			}

			if(!context->symtab)
			{
				debug_printf("ELF32: SYMTAB missing\n");
				return false;
			}

			//load all needed libraries
			for(Elf32_Dyn* d = (Elf32_Dyn*)phdrPhysMemoryAddr; d->d_tag != DT_NULL; d++)
			{
				if(d->d_tag == DT_NEEDED)
				{
					const char* libname = context->strtab + d->d_un.d_val;

					//get or load referenced image
					LoadedImage* importedImageInfo;
					if(!loader_isImageLoaded(libname, &importedImageInfo))
					{
						debug_printf("ELF: needed library %s is not yet loaded, loading...\n", libname);

						char fullPath[256] = {0};
						strncpy(fullPath, librarySearchPath, 255);
						fs_concatPath(fullPath, libname, 256);

						if(!elf_loadFile(fullPath, MemoryTypeGeexOSKernelLibrary, &importedImageInfo))
						{
							debug_printf("ELF: Error loading needed library %s\n", libname);
							return false;
						}
					}

					debug_printf("ELF: Library %s needed\n", libname);
				}
			}

			//Apply relocations (symbol references are also resolved)
			if(context->relCount > 0 && context->rel)
			{
				debug_printf("ELF: applying rel relocations\n");
				elf_elf32doRelocation(imageInformation, context->relCount, context->rel);
			}

			if(context->pltRelCount > 0 && context->pltRel)
			{
				debug_printf("ELF: applying pltrel relocations\n");
				elf_elf32doRelocation(imageInformation, context->pltRelCount, context->pltRel);
			}

			// debug_printf("Vaddr: 0x%x, Offset: 0x%x, Memsize: 0x%x, Filesize: 0x%x\n", phdr.p_vaddr, phdr.p_offset, phdr.p_memsz, phdr.p_filesz);

			return true;
		}
	}

	debug_printf("ELF32: Couldn't find DYNAMIC PHDR in ELF32 file\n");
	return false;
}

static bool elf_elf32doRelocation(LoadedImage* imageInformation, size_t relCount, Elf32_Rel* relocs)
{
	elf32_loaderContext* context = elf32_getLoaderContext(imageInformation);
	if(!context)
	{
		debug_printf("ELF32: Loader context is NULL, whut?");
		return false;
	}

	for(size_t i = 0; i < relCount; i++)
	{
		uint32_t type = ELF32_R_TYPE(relocs->r_info);
		uint32_t sym = ELF32_R_SYM(relocs->r_info);

		Elf32_Addr relocPosition = (Elf32_Addr)((relocs->r_offset - imageInformation->OriginalBase) + imageInformation->VirtualBase);

		debug_printf("ELF: Relocation type: 0x%x - symbol: 0x%x - position: 0x%x\n", type, sym, relocPosition);

		if(type == 0)
			continue;

		if(sym != 0)
		{
			const char* symbolName = (const char *)(context->strtab + context->symtab[sym].st_name);
			debug_printf("ELF: there is a symbol name assigned to this relocation: %s\n", symbolName);

			Elf32_Sym* resolvedSymbol = elf_elf32lookupSymbol(imageInformation, symbolName);
			if(resolvedSymbol)
				debug_printf("ELF32: resolved symbol to 0x%x", resolvedSymbol->st_value);
			else
				debug_printf("ELF32: failed to resolv symbol %s\n", symbolName);
		}

		switch(type)
		{
			case R_386_JMP_SLOT:
				//*(Elf32_Addr*)relocPosition = symbolAddress;
				break;

			case R_386_GLOB_DAT:
				//*(Elf32_Addr*)relocPosition = symbolAddress;
				break;

			case R_386_RELATIVE:
				if(sym)
				{
					debug_printf("ELF: R_386_RELATIVE relocation with defined symbol encountered\n");
					return false;
				}
				*(Elf32_Addr*)relocPosition += imageInformation->VirtualBase;
				break;

			case R_386_32:
				//*(Elf32_Addr*)relocPosition += symbolAddress;
				break;

			case R_386_PC32:
				//*(Elf32_Addr*)relocPosition += (symbolAddress - relocPosition);
				break;

			default:
				debug_printf("ELF: encountered unknown relocation with type 0x%x\n", type);
				return false;
		}
	}

	return true;
}

static Elf32_Sym* elf_elf32lookupSymbol(LoadedImage* imageInformation, const char* symbol)
{
	//TODO: actually resolv symbols
	return NULL;
}

static Elf32_Addr elf32_getImageSize(LoadedImage* imageInformation, Elf32_Addr* minAddr, Elf32_Addr* maxAddr)
{
	Elf32_Addr minVaddr = 0xFFFFFFFF;
	Elf32_Addr maxVaddr = 0x00000000;

	debug_printf("ELF32: Getting total image size in memory\n");

	elf32_loaderContext* context = elf32_getLoaderContext(imageInformation);
	if(!context)
	{
		debug_printf("ELF32: Loader context is NULL, whut?");
		return false;
	}

	for(size_t i = 0; i < context->phdrCount; i++)
	{
		Elf32_Phdr phdr = context->phdrs[i];

		if(phdr.p_type == PT_LOAD)
		{
			if(phdr.p_vaddr < minVaddr)
				minVaddr = phdr.p_vaddr;

			if(phdr.p_vaddr + phdr.p_memsz > maxVaddr)
				maxVaddr = phdr.p_vaddr + phdr.p_memsz;
		}
	}

	if(minVaddr > maxVaddr)
	{
		debug_printf("ELF: Unable to determine total ELF virtual image size\n");
		return 0;
	}

	if(minAddr)
		*minAddr = minVaddr;

	if(maxAddr)
		*maxAddr = maxVaddr;

	minVaddr = PAGE_START(minVaddr);
	maxVaddr = PAGE_END(maxVaddr);
	Elf32_Addr totalImageSize = maxVaddr - minVaddr;

	return totalImageSize;
}

static bool elf_loadElf64File(FILE* imageFile, MemoryType memType, LoadedImage* imageInformation)
{
	debug_printf("ELF: ELF64 is not implemented yet\n");
	return false;
}
