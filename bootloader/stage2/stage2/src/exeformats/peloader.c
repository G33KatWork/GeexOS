#include <exeformats/peloader.h>
#include <fs.h>
#include <print.h>

LIST_HEAD(loadedImages);

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

	printf("PE: Found DOS Header magic\n");
	printf("PE: Seeking to NT Headers at 0x%x\n", dosHeaderTemp.PEHeader);
	seek(imageFile, dosHeaderTemp.PEHeader, SEEK_CUR);

	IMAGE_NT_HEADERS ntHeadersTemp;
	if(read(imageFile, &ntHeadersTemp, sizeof(IMAGE_NT_HEADERS)) != sizeof(IMAGE_NT_HEADERS))
		return false;

	printf("PE: NT Headers read\n");

	if(ntHeadersTemp.Signature != IMAGE_NT_SIGNATURE)
		return false;

	printf("PE: NT Headers look valid\n");

	//TODO: check characteristics

	printf("PE: Size of image is: 0x%x\n", ntHeadersTemp.OptionalHeader.SizeOfImage);
	printf("PE: Image base is 0x%x\n", ntHeadersTemp.OptionalHeader.ImageBase);

	//Allocate memory for the pe image
	void* physicalBase = memory_allocate(ntHeadersTemp.OptionalHeader.SizeOfImage, memType);
	if(!physicalBase)
		return false;

	//seek back to start of file
	seek(imageFile, 0, SEEK_SET);


	printf("PE: Headers have size of 0x%x\n", ntHeadersTemp.OptionalHeader.SizeOfHeaders);

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

	//Map headers
	arch_map_virtual_memory_range((Address)physicalBase, virtualBase, ntHeadersTemp.OptionalHeader.SizeOfHeaders, false, false);

	//get first section
	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(ntHeaders);

	for(uint16_t i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++)
	{
		char secName[9] = {0};
		memcpy(secName, section->Name, 8);
		printf("Mapping section %s\n", secName);

		uint32_t virtualSize = section->Misc.VirtualSize;
		uint32_t sizeOfRawData = section->SizeOfRawData;

		printf("VirtualSize: 0x%x - SizeOfRawData: 0x%x\n", virtualSize, sizeOfRawData);

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
			printf("PE: Reading data from file offset 0x%x to memory offset 0x%x\n", section->PointerToRawData, section->VirtualAddress);
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

	list_add(&loadedImageInfo->Link, &loadedImages);

	if(ntHeaders->OptionalHeader.ImageBase != virtualBase)
		pe_relocateImage(loadedImageInfo, virtualBase - ntHeaders->OptionalHeader.ImageBase);

	//TODO: relocation necessary? if yes, do it
	//TODO: parse import- and export table
	//TODO: runtime linking

	pe_resolveImports(loadedImageInfo);

	if(imageInformation)
		*imageInformation = loadedImageInfo;

	return true;
}

bool pe_resolveImports(LoadedImage* image)
{
	PIMAGE_NT_HEADERS ntHeaders = pe_getHeaders((void*)image->PhysicalBase);

	IMAGE_DATA_DIRECTORY importDirectory = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	printf("PE: Import data directory is at 0x%x - Size: 0x%x\n", importDirectory.VirtualAddress, importDirectory.Size);

	return true;
}

bool pe_relocateImage(LoadedImage* image, int64_t bias)
{
	printf("PE: Relocating image %s with bias 0x%X\n", image->Name, bias);
	
	PIMAGE_NT_HEADERS ntHeaders = pe_getHeaders((void*)image->PhysicalBase);
	if(!ntHeaders)
		return false;

	if(ntHeaders->FileHeader.Characteristics & IMAGE_FILE_RELOCS_STRIPPED)
		return false;

	IMAGE_DATA_DIRECTORY relocationDirectory = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
	
	if(relocationDirectory.VirtualAddress == 0 || relocationDirectory.Size == 0)
		return true;

	PIMAGE_BASE_RELOCATION RelocationDir = (PIMAGE_BASE_RELOCATION)(image->PhysicalBase + relocationDirectory.VirtualAddress);
	PIMAGE_BASE_RELOCATION RelocationEnd = (PIMAGE_BASE_RELOCATION)((Address)RelocationDir + relocationDirectory.Size);

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
	list_for_each_entry(img, &loadedImages, Link) {
		printf("%s: Physical load address: 0x%x - Virtual base: 0x%x - Entrypoint: 0x%x - Imagesize: 0x%x\n", img->Name, img->PhysicalBase, img->VirtualBase, img->VirtualEntryPoint, img->SizeOfImage);
	}
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
