#include <exeformats/peloader.h>
#include <fs.h>
#include <print.h>

bool loadPeFile(const char* filename, MemoryType memType, Address* baseAddress, size_t* totalLength)
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

	if(totalLength)
		*totalLength = ntHeadersTemp.OptionalHeader.SizeOfImage;
	
	if(baseAddress)
		*baseAddress = (Address)physicalBase;

	//seek back to start of file
	seek(imageFile, 0, SEEK_SET);


	printf("PE: Headers have size of 0x%x\n", ntHeadersTemp.OptionalHeader.SizeOfHeaders);

	//re-read the headers into our new buffer
	if(read(imageFile, physicalBase, ntHeadersTemp.OptionalHeader.SizeOfHeaders) != ntHeadersTemp.OptionalHeader.SizeOfHeaders)
		return false;

	//get pointers to headers from new buffer
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)physicalBase;
	PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)(((Address)physicalBase) + dosHeader->PEHeader);

	Address virtualBase = ntHeaders->OptionalHeader.ImageBase;

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

		//map thi section
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

	//TODO: add loaded image to loaded-image-list
	//TODO: relocation necessary? if yes, do it
	//TODO: parse import- and export table
	//TODO: runtime linking

	return true;
}
