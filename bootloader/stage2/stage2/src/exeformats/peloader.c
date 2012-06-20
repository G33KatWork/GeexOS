#include <exeformats/peloader.h>
#include <fs.h>
#include <print.h>

LIST_HEAD(pe_loadedImages);

static const char* librarySearchPath;

bool pe_loadFile(const char* filename, MemoryType memType, LoadedImage** imageInformation)
{
	IMAGE_DOS_HEADER dosHeaderTemp;

	FILE* imageFile = open(filename);
	if(!imageFile)
		return false;

	if(read(imageFile, &dosHeaderTemp, sizeof(IMAGE_DOS_HEADER)) != sizeof(IMAGE_DOS_HEADER))
		return false;

	if(dosHeaderTemp.Magic != IMAGE_DOS_SIGNATURE)
		return false;

	debug_printf("PE: Found DOS Header magic\n");
	debug_printf("PE: Seeking to NT Headers at 0x%x\n", dosHeaderTemp.PEHeader);
	seek(imageFile, dosHeaderTemp.PEHeader, SEEK_CUR);

	IMAGE_NT_HEADERS ntHeadersTemp;
	if(read(imageFile, &ntHeadersTemp, sizeof(IMAGE_NT_HEADERS)) != sizeof(IMAGE_NT_HEADERS))
		return false;

	debug_printf("PE: NT Headers read\n");

	if(ntHeadersTemp.Signature != IMAGE_NT_SIGNATURE)
		return false;

	debug_printf("PE: NT Headers look valid\n");

	//TODO: check characteristics

	debug_printf("PE: Size of image is: 0x%x\n", ntHeadersTemp.OptionalHeader.SizeOfImage);
	debug_printf("PE: Image base is 0x%x\n", ntHeadersTemp.OptionalHeader.ImageBase);

	//Allocate memory for the pe image
	void* physicalBase = memory_allocate(ntHeadersTemp.OptionalHeader.SizeOfImage, memType);
	if(!physicalBase)
		return false;

	//seek back to start of file
	seek(imageFile, 0, SEEK_SET);


	debug_printf("PE: Headers have size of 0x%x\n", ntHeadersTemp.OptionalHeader.SizeOfHeaders);

	//re-read the headers into our new buffer
	if(read(imageFile, physicalBase, ntHeadersTemp.OptionalHeader.SizeOfHeaders) != ntHeadersTemp.OptionalHeader.SizeOfHeaders)
		return false;

	//get pointers to headers from new buffer
	PIMAGE_NT_HEADERS ntHeaders = pe_getHeaders(physicalBase);
	if(!ntHeaders)
		return false;

	Address virtualBase = ntHeaders->OptionalHeader.ImageBase;

	//HACK FOR RELOCATION TESTING!
	//virtualBase = 0xE0000000;

	//TODO: search alternative place if memory is in use
	if(!arch_is_virtual_memory_range_free(virtualBase, ntHeadersTemp.OptionalHeader.SizeOfImage))
	{
		debug_printf("PE: virtual base address %x is already in use\n", virtualBase);
		virtualBase = arch_find_free_virtual_memory_region(virtualBase, ntHeadersTemp.OptionalHeader.SizeOfImage);
		debug_printf("PE: using virtual base %x\n", virtualBase);

		if(virtualBase == 0)
			return false;
	}

	//Map headers
	arch_map_virtual_memory_range((Address)physicalBase, virtualBase, ntHeadersTemp.OptionalHeader.SizeOfHeaders, false, false);

	//get first section
	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(ntHeaders);

	for(uint16_t i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++)
	{
		char secName[9] = {0};
		memcpy(secName, section->Name, 8);
		debug_printf("Mapping section %s\n", secName);

		uint32_t virtualSize = section->Misc.VirtualSize;
		uint32_t sizeOfRawData = section->SizeOfRawData;

		debug_printf("VirtualSize: 0x%x - SizeOfRawData: 0x%x\n", virtualSize, sizeOfRawData);

		if(virtualSize == 0)
			virtualSize = sizeOfRawData;

		//do some bounds checking on data sizes
		if(section->PointerToRawData == 0)
			sizeOfRawData = 0;
		else
		{
			if(sizeOfRawData > virtualSize)
				sizeOfRawData = virtualSize;
		}

		//is there any data to read into memory?
		if(sizeOfRawData != 0)
		{
			debug_printf("PE: Reading data from file offset 0x%x to memory offset 0x%x\n", section->PointerToRawData, section->VirtualAddress);
			//seek to file offset
			seek(imageFile, section->PointerToRawData, SEEK_SET);

			//read into memory at RVA for current section
			if(read(imageFile, (void*)(((Address)physicalBase) + section->VirtualAddress), sizeOfRawData) != sizeOfRawData)
				return false;
		}

		if(sizeOfRawData < virtualSize)
			memset((void*)(((Address)physicalBase) + section->VirtualAddress + sizeOfRawData), 0, virtualSize - sizeOfRawData);

		//map this section into virtual memory
		//TODO: check if memory free and look for other place
		arch_map_virtual_memory_range(
			((Address)physicalBase) + section->VirtualAddress,
			virtualBase + section->VirtualAddress,
			virtualSize,
			section->Characteristics & IMAGE_SCN_MEM_WRITE,
			section->Characteristics & IMAGE_SCN_MEM_EXECUTE
		);

		section++;
	}

	close(imageFile);

	//add loaded image to loaded-image-list
	LoadedImage* loadedImageInfo = malloc(sizeof(LoadedImage));
	strncpy(loadedImageInfo->Name, fs_getFilename(filename), 31);
	loadedImageInfo->Name[31] = 0;
	loadedImageInfo->PhysicalBase = (Address)physicalBase;
	loadedImageInfo->VirtualBase = virtualBase;
	loadedImageInfo->VirtualEntryPoint = virtualBase + ntHeaders->OptionalHeader.AddressOfEntryPoint;
	loadedImageInfo->SizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;

	list_add(&loadedImageInfo->Link, &pe_loadedImages);

	//relocate the image if necessary
	if(ntHeaders->OptionalHeader.ImageBase != virtualBase)
		if(!pe_relocateImage(loadedImageInfo, virtualBase - ntHeaders->OptionalHeader.ImageBase))
			return false;

	//resolve imports recursively
	if(!pe_resolveImports(loadedImageInfo))
		return false;

	if(imageInformation)
		*imageInformation = loadedImageInfo;

	return true;
}

bool pe_resolveImports(LoadedImage* image)
{
	size_t importDirectorySize;
	PIMAGE_IMPORT_DESCRIPTOR importDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)pe_getDirectoryEntry(image, IMAGE_DIRECTORY_ENTRY_IMPORT, &importDirectorySize);

	//loop through all imported images
	for(;importDescriptor->Name != 0 && importDescriptor->FirstThunk != 0; importDescriptor++)
	{
		char* importName = (char*)(importDescriptor->Name + image->PhysicalBase);
		debug_printf("PE: handling import from %s\n", importName);

		//if we are referencing to our own image, ignore it
		if(stricmp(importName, image->Name) == 0)
			continue;

		//get or load referenced image
		LoadedImage* importedImageInfo;
		if(!pe_isImageLoaded(importName, &importedImageInfo))
		{
			debug_printf("PE: imported library %s is not yet loaded, loading...\n", importName);

			char fullPath[256] = {0};
			strncpy(fullPath, librarySearchPath, 255);
			fs_concatPath(fullPath, importName, 256);

			if(!pe_loadFile(fullPath, MemoryTypeGeexOSKernelLibrary, &importedImageInfo))
			{
				debug_printf("PE: Error loading imported library %s\n", importName);
				return false;
			}
		}

		//get export directory from imported dll
		size_t exportDirectorySize;
		PIMAGE_EXPORT_DIRECTORY exportDirectory = (PIMAGE_EXPORT_DIRECTORY)pe_getDirectoryEntry(importedImageInfo, IMAGE_DIRECTORY_ENTRY_EXPORT, &exportDirectorySize);
		if(!exportDirectory)
			return false;

		debug_printf("PE: export directory base: %x\n", exportDirectory->Base);

		//loop through all import thunks inside this import descriptor
		PIMAGE_THUNK_DATA importThunk = (PIMAGE_THUNK_DATA)(image->PhysicalBase + importDescriptor->FirstThunk);
		while(importThunk->u1.AddressOfData != 0)
		{
			bool isOrdinalImport = IMAGE_THUNK_DATA_IS_ORDINAL(importThunk->u1.Ordinal);
			debug_printf("PE: handling import thunk 0x%x - import by ordinal: %s\n", importThunk->u1.Ordinal, isOrdinalImport?"true":"false");

			uint32_t ordinal;

			if(isOrdinalImport)
				ordinal = IMAGE_GET_ORDINAL(importThunk->u1.Ordinal) - exportDirectory->Base;
			else
			{
				PIMAGE_IMPORT_BY_NAME namedImport = (PIMAGE_IMPORT_BY_NAME)(importThunk->u1.AddressOfData + image->PhysicalBase);
				debug_printf("PE: named import: Hint: 0x%x - Name: %s\n", namedImport->Hint, namedImport->Name);

				uint32_t* nameTable = (uint32_t*)(exportDirectory->AddressOfNames + importedImageInfo->PhysicalBase);
				uint16_t* ordinalTable = (uint16_t*)(exportDirectory->AddressOfNameOrdinals + importedImageInfo->PhysicalBase);

				uint16_t hint = namedImport->Hint;

				char* exportName = (char*)(nameTable[hint] + importedImageInfo->PhysicalBase);
				if(hint < exportDirectory->NumberOfNames && strcmp(exportName, namedImport->Name) == 0)
					ordinal = ordinalTable[hint];
				else
				{
					debug_printf("PE: Hint didn't lead to anything, searching by name...\n");

					uint32_t min = 0;
					uint32_t max = exportDirectory->NumberOfNames-1;
					uint32_t mid;

					while(max >= min)
					{
						mid = (min + max)/2;
						exportName = (char*)(nameTable[mid] + importedImageInfo->PhysicalBase);

						int cmpresult = strcmp(exportName, namedImport->Name);

						if(cmpresult > 0)
							max = mid - 1;
						else if(cmpresult < 0)
							min = mid + 1;
						else
							break;
					}

					if(max < min)
					{
						debug_printf("PE: Export %s not found\n", namedImport->Name);
						return false;
					}

					ordinal = ordinalTable[mid];
				}
			}

			if(ordinal >= exportDirectory->NumberOfFunctions)
			{
				debug_printf("PE: Determined imported ordinal 0x%x is invalid!\n", ordinal);
				return false;
			}

			uint32_t* functionTable = (uint32_t*)(exportDirectory->AddressOfFunctions + importedImageInfo->PhysicalBase);
			importThunk->u1.Function = (Address)(functionTable[ordinal] + importedImageInfo->VirtualBase);

			debug_printf("PE: determined function to be at 0x%x\n", (functionTable[ordinal] + importedImageInfo->VirtualBase));

			//TODO: check forwarder

			importThunk++;
		}

		debug_printf("AddressOfFunctions: %x - AddressOfNames: %x - AddressOfNameOrdinals: %x\n", exportDirectory->AddressOfFunctions, exportDirectory->AddressOfNames, exportDirectory->AddressOfNameOrdinals);
	}

	return true;
}

bool pe_relocateImage(LoadedImage* image, int64_t bias)
{
	debug_printf("PE: Relocating image %s with bias 0x%X\n", image->Name, bias);
	
	size_t directorySize;
	PIMAGE_BASE_RELOCATION RelocationDir = (PIMAGE_BASE_RELOCATION)pe_getDirectoryEntry(image, IMAGE_DIRECTORY_ENTRY_BASERELOC, &directorySize);
	PIMAGE_BASE_RELOCATION RelocationEnd = (PIMAGE_BASE_RELOCATION)((Address)RelocationDir + directorySize);

	//check if relocations are available
	if(!RelocationDir)
		return true;

	//Process all blocks in the directory
	while(RelocationDir < RelocationEnd && RelocationDir->SizeOfBlock > 0)
	{
		size_t relocationCount = (RelocationDir->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(uint16_t);
		Address address = image->PhysicalBase + RelocationDir->VirtualAddress;
		uint16_t* TypeOffset = (uint16_t*)(RelocationDir + 1);

		//printf("PE: Having 0x%x relocations at 0x%x. Type offset: 0x%x\n", relocationCount, address, *TypeOffset);

		//Process all relocations in this block
		for(size_t i = 0; i < relocationCount; i++)
		{
			int16_t Offset = *TypeOffset & 0xFFF;
			uint16_t Type = *TypeOffset >> 12;

			uint16_t* ptr16;
			uint32_t* ptr32;
			uint64_t* ptr64;

			switch(Type)
			{
				case IMAGE_REL_BASED_ABSOLUTE:
					break;

				case IMAGE_REL_BASED_HIGH:
					ptr16 = (uint16_t*)(address + Offset);
					*ptr16 = (uint16_t)(((*ptr16 << 16) + (bias & 0xFFFFFFFF)) >> 16);
					break;
				
				case IMAGE_REL_BASED_LOW:
					ptr16 = (uint16_t*)(address + Offset);
					*ptr16 = (uint16_t)(*ptr16 + (uint16_t)(bias & 0xFFFF));
					break;
				
				case IMAGE_REL_BASED_HIGHLOW:
					ptr32 = (uint32_t*)(address + Offset);
					*ptr32 = *ptr32 + (bias & 0xFFFFFFFF);
					break;
				
				case IMAGE_REL_BASED_DIR64:
					ptr64 = (uint64_t*)(address + Offset);
					*ptr64 = *ptr64 + bias;
					break;

				default:
					printf("PE: Unknown relocation type: 0x%x\n", Type);
					return false;
			}

			TypeOffset++;
		}

		RelocationDir = (PIMAGE_BASE_RELOCATION)TypeOffset;
	}

	return true;
}

void pe_printLoadedImages()
{
	LoadedImage *img;
	list_for_each_entry(img, &pe_loadedImages, Link) {
		debug_printf("%s: Physical load address: 0x%x - Virtual base: 0x%x - Entrypoint: 0x%x - Imagesize: 0x%x\n", img->Name, img->PhysicalBase, img->VirtualBase, img->VirtualEntryPoint, img->SizeOfImage);
	}
}

int pe_getLoadedImageCount()
{
	//Yeah! O(n) -_-
	int i = 0;
	LoadedImage *img;
	list_for_each_entry(img, &pe_loadedImages, Link) {
		i++;
	}

	return i;
}

bool pe_isImageLoaded(char* name, LoadedImage** imageInfo)
{
	LoadedImage* image;
	list_for_each_entry(image, &pe_loadedImages, Link) {
		if(stricmp(name, image->Name) == 0)
		{
			if(imageInfo)
				*imageInfo = image;

			return true;
		}
	}

	return false;
}

void pe_setLibrarySearchPath(const char* path)
{
	//TODO: allow more than one path
	librarySearchPath = path;
}

PIMAGE_NT_HEADERS pe_getHeaders(void* base)
{
	if(!base)
		return NULL;

    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)base;
    
    if(dosHeader->Magic != IMAGE_DOS_SIGNATURE)
    	return NULL;

    //TODO: check offset
	PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)(((Address)base) + dosHeader->PEHeader);
	if(ntHeaders->Signature != IMAGE_NT_SIGNATURE)
		return NULL;

	return ntHeaders;
}

void* pe_getDirectoryEntry(LoadedImage* image, uint16_t directoryIndex, size_t* directorySize)
{
	PIMAGE_NT_HEADERS ntHeaders = pe_getHeaders((void*)image->PhysicalBase);
	if(!ntHeaders)
		return NULL;

	if(directoryIndex >= ntHeaders->OptionalHeader.NumberOfRvaAndSizes)
		return NULL;

	Address rva = ntHeaders->OptionalHeader.DataDirectory[directoryIndex].VirtualAddress;
	if(rva == 0)
		return NULL;

	*directorySize = ntHeaders->OptionalHeader.DataDirectory[directoryIndex].Size;

	return (void*)(image->PhysicalBase + rva);
}
