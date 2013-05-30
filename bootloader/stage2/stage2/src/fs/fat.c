#include <fs/fat.h>
#include <lib.h>
#include <endianess.h>
#include <print.h>
#include <arch.h>

static FAT_TYPE fat_determine_type(FAT_BPB* bpb);
static inline const char* fat_type_to_string(FAT_TYPE t);

static inline FatVolumeInformation* fat_get_device_context(FilesystemMount* fsmount);
static inline void fat_set_device_context(FilesystemMount* fsmount, FatVolumeInformation* ctx);
static inline FatFileInfo* fat_get_file_context(FILE* file);
static inline void fat_set_file_context(FILE* file, FatFileInfo* ctx);

const FilesystemOperations fat_ops = {
	fat_open,
	fat_close,
	fat_read,
	fat_seek
};

FilesystemMount* fat_mount(DiskDevice* device)
{
	debug_printf("FAT: Probing for FAT on device %s\n", device->name);

	//read the first sector for the BPB
	DiskGeometry geometry;
	if(!device->get_disk_geometry(device, &geometry))
		return NULL;

	uint8_t firstSector[geometry.BytesPerSector];
	if(!device->read_sectors(device, 0, 1, firstSector))
		return NULL;

	FAT_BPB* bpb = (FAT_BPB*)firstSector;

	if( memcmp(bpb->FAT.FAT12_16.FilesystemType, "FAT12   ", 8) &&
		memcmp(bpb->FAT.FAT12_16.FilesystemType, "FAT16   ", 8) &&
		memcmp(bpb->FAT.FAT32.FilesystemType, "FAT32   ", 8)
	  ) return NULL;

	debug_printf("FAT: FS on %s seems to be some kind of FAT\n", device->name);

	FatVolumeInformation* info = malloc(sizeof(FatVolumeInformation));
	memset(info, 0, sizeof(FatVolumeInformation));

	info->FatType = fat_determine_type(bpb);
	debug_printf("FAT: It is %s\n", fat_type_to_string(info->FatType));

	if(info->FatType == FAT32)
	{
		info->BytesPerSector = LE_TO_HOST16(bpb->BytesPerSector);
		info->SectorsPerCluster = bpb->SectorsPerCluster;
		info->FatSectorStart = LE_TO_HOST16(bpb->ReservedSectors);
		info->ActiveFatSectorStart = info->FatSectorStart + 
			((LE_TO_HOST16(bpb->FAT.FAT32.ExtendedFlags) & 0x80) ? 
				((LE_TO_HOST16(bpb->FAT.FAT32.ExtendedFlags) & 0x0f) * LE_TO_HOST32(bpb->FAT.FAT32.SectorsPerFatBig))
				 : 
				0
			);

		info->NumberOfFats = bpb->NumberOfFats;
		info->SectorsPerFat = LE_TO_HOST32(bpb->FAT.FAT32.SectorsPerFatBig);

		//info->RootDirSectorStart = ...;
		//info->RootDirSectors = ...;

		info->RootDirStartCluster = LE_TO_HOST32(bpb->FAT.FAT32.RootDirStartCluster);
		info->DataSectorStart = info->FatSectorStart + info->NumberOfFats * info->SectorsPerFat;
	}
	else
	{
		info->BytesPerSector = LE_TO_HOST16(bpb->BytesPerSector);
		info->SectorsPerCluster = bpb->SectorsPerCluster;
		info->FatSectorStart = LE_TO_HOST16(bpb->ReservedSectors);
		info->ActiveFatSectorStart = info->FatSectorStart;
		info->NumberOfFats = bpb->NumberOfFats;
		info->SectorsPerFat = LE_TO_HOST16(bpb->SectorsPerFat);

		info->RootDirSectorStart = info->FatSectorStart + info->NumberOfFats * info->SectorsPerFat;
		info->RootDirSectors = ((LE_TO_HOST16(bpb->RootDirEntries) * 32) + (info->BytesPerSector - 1)) / info->BytesPerSector;

		//info->RootDirStartCluster = ...
		info->DataSectorStart = info->RootDirSectorStart + info->RootDirSectors;
	}

	FilesystemMount* fsmount = (FilesystemMount*)malloc(sizeof(FilesystemMount));
	fsmount->fsops = &fat_ops;
	fsmount->Device = device;
	fsmount->umount = fat_umount;

	fat_set_device_context(fsmount, info);

	debug_printf("FAT: FatType: %s\n", fat_type_to_string(info->FatType));
	debug_printf("FAT: BytesPerSector: %d\n", info->BytesPerSector);
	debug_printf("FAT: SectorsPerCluster: %d\n", info->SectorsPerCluster);
	debug_printf("FAT: FatSectorStart: %d\n", info->FatSectorStart);
	debug_printf("FAT: ActiveFatSectorStart: %d\n", info->ActiveFatSectorStart);
	debug_printf("FAT: NumberOfFats: %d\n", info->NumberOfFats);
	debug_printf("FAT: SectorsPerFat: %d\n", info->SectorsPerFat);
	debug_printf("FAT: RootDirSectorStart: %d\n", info->RootDirSectorStart);
	debug_printf("FAT: RootDirSectors: %d\n", info->RootDirSectors);
	debug_printf("FAT: RootDirStartCluster: %d\n", info->RootDirStartCluster);
	debug_printf("FAT: DataSectorStart: %d\n", info->DataSectorStart);

	return fsmount;
}

void fat_umount(FilesystemMount* mount)
{
	//FIXME: close all open files
	if(mount)
	{
		FatVolumeInformation* volume = fat_get_device_context(mount);
		if(volume)
		{
			free(volume);
		}

		free(mount);
	}
}


bool fat_close(FILE* f)
{
	if(f)
	{
		FatFileInfo* fi = fat_get_file_context(f);
		if(fi)
		{
			if(fi->FATChain)
				free(fi->FATChain);

			free(fi);
		}

		free(f);
	}
	
	return true;
}

FILE* fat_open(const char* path, FilesystemMount* mount)
{
	debug_printf("FAT: Opening file %s on device %s\n", path, mount->Device->name);
	FILE* file = malloc(sizeof(FILE));
	file->mount = mount;
	file->filePointer = 0;

	FatFileInfo* fileInfo = malloc(sizeof(FatFileInfo));

	if(!fat_findFile(mount, path, fileInfo))
		return NULL;

	fat_set_file_context(file, fileInfo);

	return file;
}

size_t fat_read(FILE* f, void* buffer, size_t s)
{
	size_t bytesRead = 0;

	if(!f)
		return -1;

	//get some metadata stuff about the fat volume and mount we are reading from
	FilesystemMount* mount = f->mount;
	if(!mount)
		return -2;

	FatVolumeInformation* volume = fat_get_device_context(mount);
	if(!volume)
		return -3;

	FatFileInfo* fi = fat_get_file_context(f);
	if(!fi)
		return -4;

	//EOF?
	if(f->filePointer >= fi->Filesize)
		return 0;

	//adjust read size if necessary
	if(f->filePointer + s > fi->Filesize)
		s = fi->Filesize - f->filePointer;

	uint32_t bytesPerCluster = volume->SectorsPerCluster * volume->BytesPerSector;

	//read until we are on a cluster boundary
	if(f->filePointer % bytesPerCluster)
	{
		uint32_t clusterNumber = fi->FATChain[f->filePointer / bytesPerCluster];
		uint32_t offsetInCluster = f->filePointer % bytesPerCluster;
		uint32_t lengthInCluster = (s > (bytesPerCluster - offsetInCluster)) ? (bytesPerCluster - offsetInCluster) : s;


		if(!fat_readPartialCluster(mount, clusterNumber, offsetInCluster, lengthInCluster, buffer))
			return -5;

		bytesRead += lengthInCluster;

		s -= lengthInCluster;
		f->filePointer += lengthInCluster;
		buffer = (void*)(((uintptr_t)buffer) + lengthInCluster);
	}

	//read all the full clusters
	if(s > 0)
	{
		uint32_t numberOfClusters = s / bytesPerCluster;

		if(numberOfClusters > 0)
		{
			uint32_t clusterNumber = fi->FATChain[f->filePointer / bytesPerCluster];

			if(!fat_readClusterChain(mount, clusterNumber, numberOfClusters, buffer))
				return false;

			bytesRead += numberOfClusters * bytesPerCluster;

			s -= numberOfClusters * bytesPerCluster;
			f->filePointer += numberOfClusters * bytesPerCluster;
			buffer = (void*)(((uintptr_t)buffer) + (numberOfClusters * bytesPerCluster));
		}
	}

	//read the remaining bytes of the last cluster if needed
	if(s > 0)
	{
		uint32_t clusterNumber = fi->FATChain[f->filePointer / bytesPerCluster];

		if(!fat_readPartialCluster(mount, clusterNumber, 0, s, buffer))
			return -7;

		bytesRead += s;

		s -= s;
		f->filePointer += s;
		buffer = (void*)(((uintptr_t)buffer) + s);
	}

	return bytesRead;
}

void fat_seek(FILE* f, long offset, SeekMode mode)
{
	//TODO: move into fs?
	if(f)
	{
		FatFileInfo* fi = fat_get_file_context(f);
		if(!fi)
			return;

		switch(mode)
		{
			case SEEK_SET:
				if(offset < 0)
					f->filePointer = 0;
				else if(offset > (int64_t)fi->Filesize)
					f->filePointer = fi->Filesize;
				else
					f->filePointer = offset;
				break;

			case SEEK_END:
				if(offset >= 0)
					f->filePointer = fi->Filesize;
				else if((int64_t)fi->Filesize + offset < 0)
					f->filePointer = 0;
				else
					f->filePointer = fi->Filesize + offset;
				break;

			case SEEK_CUR:
				if((int64_t)f->filePointer + offset < 0)
					f->filePointer = 0;
				else if(f->filePointer + offset > fi->Filesize)
					f->filePointer = fi->Filesize;
				else
					f->filePointer += offset;
				break;
		}
	}

	debug_printf("FAT: Seeking.. Mode: %x - offset: %x - new fp: %x\n", mode, offset, f->filePointer);
}

bool fat_findFile(FilesystemMount* mount, const char* filename, FatFileInfo* fileinfo)
{
	debug_printf("FAT: trying to locate file %s on %s\n", filename, mount->Device->name);

	char pathPart[261];	//One long vfat filename can have max 260 chars
	void* directoryBuffer;
	uint32_t directoryStartCluster = 0;
	size_t directorySize;

	uint32_t numPathParts = fs_getNumPathParts(filename);
	for(uint32_t i = 0; i < numPathParts; i++)
	{
		fs_getFirstElementFromPath(filename, pathPart);

		for(; *filename != '\\' && *filename != '/' && *filename != '\0'; filename++)
		{}
		filename++;

		directoryBuffer = fat_getDirectoryContents(mount, directoryStartCluster, &directorySize, (i == 0));
		if(!directoryBuffer)
			return false;

		if(!fat_searchDirectoryForFile(mount, directoryBuffer, directorySize, pathPart, fileinfo))
		{
			free(directoryBuffer);
			return false;
		}

		//Free the directory contents, fat_getDirectoryContents() allocated it
		free(directoryBuffer);

		if(numPathParts > i+1)
		{
			//is the file we found a directory? if not, bail out
			if(!(fileinfo->Attributes & ATTR_DIRECTORY))
			{
				free(fileinfo->FATChain);
				return false;
			}

			directoryStartCluster = fileinfo->FATChain[0];
			free(fileinfo->FATChain);
			fileinfo->FATChain = NULL;
		}
	}

	return true;
}

bool fat_searchDirectoryForFile(FilesystemMount* mount, void* directoryContents, size_t directorySize, const char* filename, FatFileInfo* fileinfo)
{
	debug_printf("FAT: searching directory for file %s on device %s\n", filename, mount->Device->name);

	char longNameBuffer[265];
	char shortNameBuffer[20];
	uint32_t currentEntry;

	memset(longNameBuffer, 0, sizeof(longNameBuffer));
	memset(shortNameBuffer, 0, sizeof(shortNameBuffer));

	uint32_t entryCount = directorySize / sizeof(FatDirEntry);

	for(currentEntry = 0; currentEntry < entryCount; directoryContents = (void*)(((FatDirEntry*)directoryContents) + 1))
	{
		FatLfnDirentry* curLfnEntry = (FatLfnDirentry*)directoryContents;
		FatDirEntry* curEntry = (FatDirEntry*)directoryContents;
		//FIXME: swap members... bla

		//End Of Directory?
		if(curEntry->Filename[0] == '\0')
			return false;

		//deleted file?
		if(curEntry->Filename[0] == '\xE5')
		{
			memset(longNameBuffer, 0, sizeof(longNameBuffer));
			memset(shortNameBuffer, 0, sizeof(shortNameBuffer));
			continue;
		}

		//handle long filenames
		if(curEntry->Attributes == ATTR_LONG_NAME)
		{
			//entry marked as deleted?
			if(curLfnEntry->SequenceNumber & 0x80)
				continue;

			curLfnEntry->SequenceNumber &= 0x3F;
			curLfnEntry->SequenceNumber--;

			if (curLfnEntry->Name0_4[0] != 0xFFFF)
				longNameBuffer[0 + (curLfnEntry->SequenceNumber * 13)] = (char)curLfnEntry->Name0_4[0];
			
			if (curLfnEntry->Name0_4[1] != 0xFFFF)
				longNameBuffer[1 + (curLfnEntry->SequenceNumber * 13)] = (char)curLfnEntry->Name0_4[1];
			
			if (curLfnEntry->Name0_4[2] != 0xFFFF)
				longNameBuffer[2 + (curLfnEntry->SequenceNumber * 13)] = (char)curLfnEntry->Name0_4[2];
			
			if (curLfnEntry->Name0_4[3] != 0xFFFF)
				longNameBuffer[3 + (curLfnEntry->SequenceNumber * 13)] = (char)curLfnEntry->Name0_4[3];
			
			if (curLfnEntry->Name0_4[4] != 0xFFFF)
				longNameBuffer[4 + (curLfnEntry->SequenceNumber * 13)] = (char)curLfnEntry->Name0_4[4];
			
			if (curLfnEntry->Name5_10[0] != 0xFFFF)
				longNameBuffer[5 + (curLfnEntry->SequenceNumber * 13)] = (char)curLfnEntry->Name5_10[0];
			
			if (curLfnEntry->Name5_10[1] != 0xFFFF)
				longNameBuffer[6 + (curLfnEntry->SequenceNumber * 13)] = (char)curLfnEntry->Name5_10[1];
			
			if (curLfnEntry->Name5_10[2] != 0xFFFF)
				longNameBuffer[7 + (curLfnEntry->SequenceNumber * 13)] = (char)curLfnEntry->Name5_10[2];
			
			if (curLfnEntry->Name5_10[3] != 0xFFFF)
				longNameBuffer[8 + (curLfnEntry->SequenceNumber * 13)] = (char)curLfnEntry->Name5_10[3];
			
			if (curLfnEntry->Name5_10[4] != 0xFFFF)
				longNameBuffer[9 + (curLfnEntry->SequenceNumber * 13)] = (char)curLfnEntry->Name5_10[4];
			
			if (curLfnEntry->Name5_10[5] != 0xFFFF)
				longNameBuffer[10 + (curLfnEntry->SequenceNumber * 13)] = (char)curLfnEntry->Name5_10[5];
			
			if (curLfnEntry->Name11_12[0] != 0xFFFF)
				longNameBuffer[11 + (curLfnEntry->SequenceNumber * 13)] = (char)curLfnEntry->Name11_12[0];

			if (curLfnEntry->Name11_12[1] != 0xFFFF)
				longNameBuffer[12 + (curLfnEntry->SequenceNumber * 13)] = (char)curLfnEntry->Name11_12[1];

			continue;
		}

		//skip volume label
		if(curEntry->Attributes & ATTR_VOLUMENAME)
		{
			memset(longNameBuffer, 0, sizeof(longNameBuffer));
			memset(shortNameBuffer, 0, sizeof(shortNameBuffer));
			continue;
		}

		fat_parseShortFilename(curEntry, shortNameBuffer);
		//printf("FAT: Found file %s in a directory\n", shortNameBuffer);
		if(stricmp(filename, shortNameBuffer) == 0 || stricmp(filename, longNameBuffer) == 0)
		{
			uint32_t startCluster = (curEntry->ClusterHigh << 16) | curEntry->ClusterLow;
			debug_printf("FAT: Found file %s with size 0x%x start cluster 0x%x\n", filename, curEntry->Size, startCluster);

			fileinfo->Attributes = curEntry->Attributes;
			fileinfo->Filesize = curEntry->Size;
			fileinfo->FATChain = fat_getFatChain(mount, startCluster);

			return true;
		}

		memset(longNameBuffer, 0, sizeof(longNameBuffer));
		memset(shortNameBuffer, 0, sizeof(shortNameBuffer));
		continue;
	}

	return false;
}

void fat_parseShortFilename(FatDirEntry* entry, char* buffer)
{
	memset(buffer, 0, 13);

	if(entry->Filename[0] == 0x05)
		entry->Filename[0] = (char)0xE5;

	uint32_t i = 0;
	while(i < 8)
	{
		if(entry->Filename[i] == ' ')
			break;

		buffer[i] = entry->Filename[i];
		i++;
	}

	if(entry->Filename[8] != ' ')
	{
		buffer[i++] = '.';
		buffer[i++] = (entry->Filename[8] == ' ') ? '\0' : entry->Filename[8];
		buffer[i++] = (entry->Filename[9] == ' ') ? '\0' : entry->Filename[9];
		buffer[i++] = (entry->Filename[10] == ' ') ? '\0' : entry->Filename[10];
	}
}

void* fat_getDirectoryContents(FilesystemMount* mount, uint32_t startCluster, size_t* directorySize, bool rootDirectory)
{
	debug_printf("FAT: getting directory contents: startCluster: %x, rootDir: %s, device: %s\n", startCluster, rootDirectory ? "true": "false", mount->Device->name);
	FatVolumeInformation* volume = fat_get_device_context(mount);

	//for FAT32 the root dir is just another subdir
	if(rootDirectory && volume->FatType == FAT32)
	{
		startCluster = volume->RootDirStartCluster;
		rootDirectory = false;
	}

	if(rootDirectory)
		*directorySize = volume->RootDirSectors * volume->BytesPerSector;
	else
		*directorySize = fat_countClustersInChain(mount, startCluster) * volume->SectorsPerCluster * volume->BytesPerSector;

	debug_printf("FAT: directory seems to have size 0x%x\n", *directorySize);

	//this malloc is freed later in fat_findFile()
	void* directoryContents = malloc(*directorySize);
	if(!directoryContents)
		return NULL;

	if(rootDirectory)
	{
		debug_printf("FAT: we are reading a root dir with 0x%x sectors starting from sector 0x%x\n", volume->RootDirSectors, volume->RootDirSectorStart);
		if(!mount->Device->read_sectors(mount->Device, volume->RootDirSectorStart, volume->RootDirSectors, directoryContents))
		{
			free(directoryContents);
			return false;
		}
	}
	else
	{
		if(!fat_readClusterChain(mount, startCluster, 0xFFFFFFFF, directoryContents))
		{
			free(directoryContents);
			return false;
		}
	}

	return directoryContents;
}

bool fat_readClusterChain(FilesystemMount* mount, uint32_t startCluster, uint32_t maxClusters, void* buffer)
{
	debug_printf("FAT: Reading cluster chain starting from cluster 0x%x on %s\n", startCluster, mount->Device->name);

	FatVolumeInformation* volume = fat_get_device_context(mount);
	void* tmpbuf[volume->SectorsPerCluster * volume->BytesPerSector];

	uint32_t dataStartSector;

	while(maxClusters > 0)
	{
		dataStartSector = ((startCluster - 2) * volume->SectorsPerCluster) + volume->DataSectorStart;

		if(!mount->Device->read_sectors(mount->Device, dataStartSector, volume->SectorsPerCluster, tmpbuf))
			return false;

		memcpy(buffer, tmpbuf, volume->SectorsPerCluster * volume->BytesPerSector);
		maxClusters--;

		buffer = (void*)(((uintptr_t)buffer) + (volume->SectorsPerCluster * volume->BytesPerSector));

		if(!fat_getFatEntry(mount, startCluster, &startCluster))
			return false;

		//did we reach the end?
		if((volume->FatType == FAT12 && startCluster >= 0xFF8)   ||
		   (volume->FatType == FAT16 && startCluster >= 0xFFF8)  ||
		   (volume->FatType == FAT32 && startCluster >= 0x0FFFFFF8)
		)
			break;
	}

	return true;
}

bool fat_readPartialCluster(FilesystemMount* mount, uint32_t clusterNumber, size_t offset, size_t length, void* buffer)
{
	FatVolumeInformation* volume = fat_get_device_context(mount);
	void* tmpbuf[volume->SectorsPerCluster * volume->BytesPerSector];

	uint32_t startSector = ((clusterNumber - 2) * volume->SectorsPerCluster) + volume->DataSectorStart;

	if(!mount->Device->read_sectors(mount->Device, startSector, volume->SectorsPerCluster, tmpbuf))
		return false;

	memcpy(buffer, (void*)(((uintptr_t)tmpbuf) + offset), length);

	return true;
}

bool fat_getFatEntry(FilesystemMount* mount, uint32_t cluster, uint32_t* result)
{
	FatVolumeInformation* volume = fat_get_device_context(mount);

	//printf("FAT: getting follow-up cluster for cluster 0x%x on %s\n", cluster, mount->Device->name);

	uint8_t buffer[volume->BytesPerSector * 2];

	uint32_t fat = 0;
	uint32_t fatOffset;
	uint32_t thisFatSecNum;
	uint32_t thisFatEntOffset;

	switch(volume->FatType)
	{
		case FAT12:
			fatOffset = cluster + (cluster / 2);
			thisFatSecNum = volume->ActiveFatSectorStart + (fatOffset / volume->BytesPerSector);
			thisFatEntOffset = fatOffset % volume->BytesPerSector;

			if(thisFatEntOffset == (volume->BytesPerSector - 1))
			{
				if(!mount->Device->read_sectors(mount->Device, thisFatSecNum, 2, buffer))
					return false;
			}
			else
			{
				if(!mount->Device->read_sectors(mount->Device, thisFatSecNum, 1, buffer))
					return false;
			}

			fat = *((uint16_t*) (((uintptr_t)buffer) + thisFatEntOffset));
			fat = LE_TO_HOST16(fat);

			if(cluster & 0x0001)
				fat = fat >> 4;
			else
				fat = fat & 0xFFF;
		break;

		case FAT16:
			fatOffset = (cluster * 2);
			thisFatSecNum = volume->ActiveFatSectorStart + (fatOffset / volume->BytesPerSector);
			thisFatEntOffset = (fatOffset % volume->BytesPerSector);

			if(!mount->Device->read_sectors(mount->Device, thisFatSecNum, 1, buffer))
				return false;

			fat = *((uint16_t*) (((uintptr_t)buffer) + thisFatEntOffset));
			fat = LE_TO_HOST16(fat);
		break;

		case FAT32:
			fatOffset = (cluster * 4);
			thisFatSecNum = volume->ActiveFatSectorStart + (fatOffset / volume->BytesPerSector);
			thisFatEntOffset = (fatOffset % volume->BytesPerSector);

			if(!mount->Device->read_sectors(mount->Device, thisFatSecNum, 1, buffer))
				return false;

			fat = *((uint32_t*) (((uintptr_t)buffer) + thisFatEntOffset)) & 0x0FFFFFFF;
			fat = LE_TO_HOST32(fat);
		break;

		default:
			arch_panic("Dafuq? Uknown FAT Type detected.");
	}

	*result = fat;
	return true;
}

uint32_t fat_countClustersInChain(FilesystemMount* mount, uint32_t startCluster)
{
	debug_printf("FAT: counting clusters in chain. startCluster: %x\n", startCluster);

	FatVolumeInformation* volume = fat_get_device_context(mount);

	uint32_t i = 0;
	while(true)
	{
		//did we reach the end?
		if((volume->FatType == FAT12 && startCluster >= 0xFF8)   ||
		   (volume->FatType == FAT16 && startCluster >= 0xFFF8)  ||
		   (volume->FatType == FAT32 && startCluster >= 0x0FFFFFF8)
		)
			break;

		i++;

		if(!fat_getFatEntry(mount, startCluster, &startCluster))
			return 0;
	}

	debug_printf("FAT: chain consists of %x clusters\n", i);

	return i;
}

uint32_t* fat_getFatChain(FilesystemMount* mount, uint32_t startCluster)
{
	FatVolumeInformation* volume = fat_get_device_context(mount);

	uint32_t clusterCount = fat_countClustersInChain(mount, startCluster);
	uint32_t* fatArray = malloc(clusterCount * sizeof(uint32_t));

	if(!fatArray)
		return NULL;

	for(uint32_t i = 0; i < clusterCount; i++)
	{
		fatArray[i] = startCluster;

		if((volume->FatType == FAT12 && startCluster >= 0xFF8)   ||
		   (volume->FatType == FAT16 && startCluster >= 0xFFF8)  ||
		   (volume->FatType == FAT32 && startCluster >= 0x0FFFFFF8)
		)
		{
			i++;
			break;
		}
		
		if(!fat_getFatEntry(mount, startCluster, &startCluster))
		{
			free(fatArray);
			return NULL;
		}
	}

	return fatArray;
}

static FAT_TYPE fat_determine_type(FAT_BPB* bpb)
{
	uint32_t RootDirSectors = LE_TO_HOST16(bpb->RootDirEntries) * 32 + (LE_TO_HOST16(bpb->BytesPerSector) - 1) / LE_TO_HOST16(bpb->BytesPerSector);
	uint32_t SectorsPerFat = LE_TO_HOST16(bpb->SectorsPerFat) ? LE_TO_HOST16(bpb->SectorsPerFat) : LE_TO_HOST32(bpb->FAT.FAT32.SectorsPerFatBig);
	uint32_t TotalSectors = LE_TO_HOST16(bpb->TotalSectors) ? LE_TO_HOST16(bpb->TotalSectors) : LE_TO_HOST32(bpb->TotalSectorsBig);
	uint32_t DataSectorCount = TotalSectors - (LE_TO_HOST16(bpb->ReservedSectors) + (bpb->NumberOfFats * SectorsPerFat) + RootDirSectors);

	uint32_t CountOfClusters = 0;
	if(bpb->SectorsPerCluster != 0)
		CountOfClusters = DataSectorCount / bpb->SectorsPerCluster;

	if(CountOfClusters < 4085)
		return FAT12;
	else if(CountOfClusters < 65525)
		return FAT16;
	else
		return FAT32;
}

static inline const char* fat_type_to_string(FAT_TYPE t)
{
	switch(t) {
		case FAT12:
			return "FAT12";
		case FAT16:
			return "FAT16";
		case FAT32:
			return "FAT32";
		default:
			return "WTF?";
	}
}

static inline FatVolumeInformation* fat_get_device_context(FilesystemMount* fsmount)
{
	return (FatVolumeInformation*)fsmount->fsContext;
}

static inline void fat_set_device_context(FilesystemMount* fsmount, FatVolumeInformation* ctx)
{
	fsmount->fsContext = (void*)ctx;
}

static inline FatFileInfo* fat_get_file_context(FILE* file)
{
	return (FatFileInfo*)file->fsContext;
}

static inline void fat_set_file_context(FILE* file, FatFileInfo* ctx)
{
	file->fsContext = (void*)ctx;
}
