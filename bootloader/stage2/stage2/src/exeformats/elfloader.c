#include <exeformats/elfloader.h>
#include <fs.h>
#include <print.h>
#include <arch.h>

static bool elf_getElf32Header(FILE* imageFile, LoadedImage* imageInformation);
static bool elf_getElf32Phdrs(FILE* imageFile, LoadedImage* imageInformation);

static bool elf_loadElf32File(FILE* imageFile, MemoryType memType, LoadedImage* imageInformation);
static bool elf_linkElf32File(LoadedImage* imageInformation);
static bool elf_elf32doRelocation(LoadedImage* imageInformation, size_t relCount, Elf32_Rel* relocs, LoadedImage** neededImages);
static Elf32_Sym* elf_elf32lookupSymbol(LoadedImage* imageInformation, const char* symbol, LoadedImage** neededImages);
static Elf32_Sym* elf_elf32lookupSymbolInImage(LoadedImage* imageInformation, const char* symbol);
static Elf32_Addr elf32_getImageSize(LoadedImage* imageInformation, Elf32_Addr* minAddr, Elf32_Addr* maxAddr);

static bool elf_loadElf64File(FILE* imageFile, MemoryType memType, LoadedImage* imageInformation);

static unsigned elfhash(const char* _name);

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

	//Symbolic flag
	bool has_symbolic;
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

			uintptr_t targetAddress = ((uintptr_t)physicalBase) + (phdr.p_vaddr - originalVirtualBase);

			if(phdr.p_filesz > 0)
			{
				debug_printf("ELF32: seeking to 0x%x\n", phdr.p_offset);
				seek(imageFile, phdr.p_offset, SEEK_SET);

				debug_printf("ELF32: reading 0x%x bytes to 0x%x\n", phdr.p_filesz, targetAddress);
				if(read(imageFile, (void*)targetAddress, phdr.p_filesz) != phdr.p_filesz)
				{
					debug_printf("ELF32: Error while reading segment data from ELF32 file\n");
					return false;
				}
			}

			if(phdr.p_memsz > phdr.p_filesz)
			{
				debug_printf("ELF32: zeroing memory from 0x%x to 0x%x\n", targetAddress + phdr.p_filesz, targetAddress + phdr.p_filesz + (phdr.p_memsz - phdr.p_filesz));
				memset((void*)(targetAddress + phdr.p_filesz), 0, phdr.p_memsz - phdr.p_filesz);
			}

			//Map region described by LOAD-PHDR into virtual memory
			arch_map_virtual_memory_range(
				targetAddress,
				(phdr.p_vaddr - originalVirtualBase) + virtualBase,
				phdr.p_memsz,
				(phdr.p_flags & PF_W) == PF_W,
				(phdr.p_flags & PF_X) == PF_X
			);
		}
	}

	uintptr_t entrypoint = (context->header->e_entry - originalVirtualBase) + virtualBase;

	imageInformation->PhysicalBase = (uintptr_t)physicalBase;
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

			uintptr_t phdrPhysMemoryAddr = imageInformation->PhysicalBase + (phdr.p_vaddr - imageInformation->OriginalBase);
			debug_printf("ELF32: dynamic headers may be mapped at 0x%x\n", phdrPhysMemoryAddr);

			//Check wether the DYNAMIC headers are already somewhere in memory
			//TODO: do the DYNAMIC PHDRs always get loaded through a LOAD PHDR? What if not?
			if(arch_is_virtual_memory_range_free(imageInformation->VirtualBase + (phdr.p_vaddr - imageInformation->OriginalBase), phdr.p_memsz))
			{
				debug_printf("ELF32: DYNAMIC PHDRs doesn't seem to be in memory\n");
				return false;
			}

			uint32_t neededCount = 0;

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
						context->pltRel = (Elf32_Rel*)((uintptr_t)(imageInformation->PhysicalBase + (d->d_un.d_ptr - imageInformation->OriginalBase)));
						break;

					case DT_PLTRELSZ:
						context->pltRelCount = d->d_un.d_val / sizeof(Elf32_Rel);
						break;

					case DT_REL:
						context->rel = (Elf32_Rel*)((uintptr_t)(imageInformation->PhysicalBase + (d->d_un.d_ptr - imageInformation->OriginalBase)));
						break;

					case DT_RELSZ:
						context->relCount = d->d_un.d_val / sizeof(Elf32_Rel);
						break;

					case DT_PLTGOT:
						/* Save this in case we decide to do lazy binding. We don't yet. */
						//si->plt_got = (unsigned *)(base + d->d_un.d_ptr);
						break;

					case DT_SYMTAB:
						context->symtab = (Elf32_Sym*)((uintptr_t)(imageInformation->PhysicalBase + (d->d_un.d_ptr - imageInformation->OriginalBase)));
						break;

					case DT_STRTAB:
						context->strtab = (const char*)((uintptr_t)(imageInformation->PhysicalBase + (d->d_un.d_ptr - imageInformation->OriginalBase)));
						break;

					case DT_HASH:
						context->nbucket = ((unsigned *) ((uintptr_t)(imageInformation->PhysicalBase + (d->d_un.d_ptr - imageInformation->OriginalBase))))[0];
						context->nchain = ((unsigned *) ((uintptr_t)(imageInformation->PhysicalBase + (d->d_un.d_ptr - imageInformation->OriginalBase))))[1];
						context->bucket = (unsigned *) ((uintptr_t)(imageInformation->PhysicalBase + (d->d_un.d_ptr - imageInformation->OriginalBase) + 8));
						context->chain = (unsigned *) ((uintptr_t)(imageInformation->PhysicalBase + (d->d_un.d_ptr - imageInformation->OriginalBase) + 8 + context->nbucket * 4));
						break;

					case DT_SYMBOLIC:
						context->has_symbolic = true;
						break;

					case DT_NEEDED:
						neededCount++;
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

			//allocate memory for a list of all referenced libraries + NULL-Pointer as terminator
			LoadedImage** neededImages = malloc(sizeof(LoadedImage*) * (neededCount+1));
			memset(neededImages, 0, sizeof(LoadedImage*) * (neededCount+1));

			if(neededCount > 0)
			{
				unsigned int j = 0;

				//load all needed libraries
				for(Elf32_Dyn* d = (Elf32_Dyn*)phdrPhysMemoryAddr; d->d_tag != DT_NULL; d++)
				{
					if(d->d_tag == DT_NEEDED)
					{
						const char* libname = context->strtab + d->d_un.d_val;
						debug_printf("ELF32: Library %s needed\n", libname);

						//get or load referenced image
						LoadedImage* importedImageInfo;
						if(!loader_isImageLoaded(libname, &importedImageInfo))
						{
							debug_printf("ELF32: needed library %s is not yet loaded, loading...\n", libname);

							char fullPath[256] = {0};
							strncpy(fullPath, librarySearchPath, 255);
							fs_concatPath(fullPath, libname, 256);

							if(!elf_loadFile(fullPath, MemoryTypeGeexOSKernelLibrary, &importedImageInfo))
							{
								debug_printf("ELF32: Error loading needed library %s\n", libname);
								return false;
							}
						}

						neededImages[j++] = importedImageInfo;
					}
				}
			}

			//Apply relocations (symbol references are also resolved)
			if(context->relCount > 0 && context->rel)
			{
				debug_printf("ELF32: applying rel relocations\n");
				if(!elf_elf32doRelocation(imageInformation, context->relCount, context->rel, neededImages))
				{
					free(neededImages);
					return false;
				}
			}

			if(context->pltRelCount > 0 && context->pltRel)
			{
				debug_printf("ELF32: applying pltrel relocations\n");
				if(!elf_elf32doRelocation(imageInformation, context->pltRelCount, context->pltRel, neededImages))
				{
					free(neededImages);
					return false;
				}
			}

			//Free list of all referenced libraries
			free(neededImages);

			return true;
		}
	}

	debug_printf("ELF32: Couldn't find DYNAMIC PHDR in ELF32 file\n");
	return false;
}

static bool elf_elf32doRelocation(LoadedImage* imageInformation, size_t relCount, Elf32_Rel* relocs, LoadedImage** neededImages)
{
	elf32_loaderContext* context = elf32_getLoaderContext(imageInformation);
	if(!context)
	{
		debug_printf("ELF32: Loader context is NULL, whut?");
		return false;
	}

	for(size_t i = 0; i < relCount; i++)
	{
		uint32_t type = ELF32_R_TYPE(relocs[i].r_info);
		uint32_t sym = ELF32_R_SYM(relocs[i].r_info);

		Elf32_Addr relocPosition = (Elf32_Addr)((relocs[i].r_offset - imageInformation->OriginalBase) + imageInformation->PhysicalBase);
		Elf32_Addr symbolAddress = 0;

		debug_printf("ELF32: Relocation type: 0x%x - symbol: 0x%x - position: 0x%x\n", type, sym, relocPosition);

		if(type == 0)
			continue;

		if(sym != 0)
		{
			//TODO: Weak symbols?

			const char* symbolName = (const char *)(context->strtab + context->symtab[sym].st_name);
			debug_printf("ELF32: there is a symbol name assigned to this relocation: %s\n", symbolName);

			Elf32_Sym* resolvedSymbol = elf_elf32lookupSymbol(imageInformation, symbolName, neededImages);
			if(resolvedSymbol)
			{
				symbolAddress = (resolvedSymbol->st_value - imageInformation->OriginalBase) + imageInformation->VirtualBase;
				debug_printf("ELF32: resolved symbol to 0x%x\n", resolvedSymbol->st_value);
			}
			else
			{
				debug_printf("ELF32: failed to resolv symbol %s\n", symbolName);
				return false;
			}
		}

		switch(type)
		{
			case R_386_JMP_SLOT:
				debug_printf("ELF32: R_386_JMP_SLOT relocation at 0x%x encountered, symbol address: 0x%x\n", relocPosition, symbolAddress);
				*(Elf32_Addr*)((uintptr_t)relocPosition) = symbolAddress;
				break;

			case R_386_GLOB_DAT:
				debug_printf("ELF32: R_386_GLOB_DAT relocation encountered, symbol address: 0x%x\n", symbolAddress);
				*(Elf32_Addr*)((uintptr_t)relocPosition) = symbolAddress;
				break;

			case R_386_RELATIVE:
				if(sym)
				{
					debug_printf("ELF32: R_386_RELATIVE relocation with defined symbol encountered\n");
					return false;
				}
				*(Elf32_Addr*)((uintptr_t)relocPosition) += imageInformation->VirtualBase - imageInformation->OriginalBase;
				break;

			case R_386_32:
				debug_printf("ELF32: R_386_32 relocation encountered, symbol address: 0x%x\n", symbolAddress);
				*(Elf32_Addr*)((uintptr_t)relocPosition) += symbolAddress;
				break;

			case R_386_PC32:
				debug_printf("ELF32: R_386_PC32 relocation encountered, symbol address: 0x%x\n", symbolAddress);
				*(Elf32_Addr*)((uintptr_t)relocPosition) += (symbolAddress - relocPosition);
				break;

			default:
				debug_printf("ELF32: encountered unknown relocation with type 0x%x\n", type);
				return false;
		}
	}

	return true;
}

static Elf32_Sym* elf_elf32lookupSymbol(LoadedImage* imageInformation, const char* symbol, LoadedImage** neededImages)
{
	elf32_loaderContext* context = elf32_getLoaderContext(imageInformation);
	if(!context)
	{
		debug_printf("ELF32: Loader context is NULL, whut?");
		return false;
	}

	//1. Lookup in scope of the library, if symbolic flag is set
	if(context->has_symbolic)
	{
		Elf32_Sym* sym = elf_elf32lookupSymbolInImage(imageInformation, symbol);
		if(sym)
			return sym;
	}

	//2. Lookup in scope of the main executable
	LoadedImage* mainExecutable = loader_getExecutable();
	if(mainExecutable)
	{
		Elf32_Sym* sym = elf_elf32lookupSymbolInImage(mainExecutable, symbol);
		if(sym)
			return sym;
	}

	//3. Then look up the symbol in all needed libraries
	for(LoadedImage* image = *neededImages; image; image++)
	{
		Elf32_Sym* sym = elf_elf32lookupSymbolInImage(image, symbol);
		if(sym)
			return sym;
	}

	return NULL;
}

static Elf32_Sym* elf_elf32lookupSymbolInImage(LoadedImage* imageInformation, const char* symbol)
{
	unsigned hash = elfhash(symbol);
	debug_printf("ELF32: Hash of symbol %s is 0x%x\n", symbol, hash);

	elf32_loaderContext* context = elf32_getLoaderContext(imageInformation);
	if(!context)
	{
		debug_printf("ELF32: Loader context is NULL, whut?");
		return false;
	}

	for(unsigned n = context->bucket[hash % context->nbucket]; n != 0; n = context->chain[n])
	{
		Elf32_Sym* s = context->symtab + n;
		if(strcmp(context->strtab + s->st_name, symbol)) continue;

		switch(ELF32_ST_BIND(s->st_info))
		{
			case STB_GLOBAL:
			case STB_WEAK:
				if (s->st_shndx == SHN_UNDEF)
					continue;

				debug_printf("ELF32: Found symbol %s at 0x%x in %s\n", symbol, s->st_value, imageInformation->Name);
				return s;
		}
	}

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
		debug_printf("ELF32: Unable to determine total ELF virtual image size\n");
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
	debug_printf("ELF64: ELF64 is not implemented yet\n");
	return false;
}


static unsigned elfhash(const char* _name)
{
	const unsigned char* name = (const unsigned char*) _name;
	unsigned h = 0, g;

	while(*name)
	{
		h = (h << 4) + *name++;
		g = h & 0xf0000000;
		h ^= g;
		h ^= g >> 24;
	}
	return h;
}
