#include <fs/fat.h>
#include <lib.h>
#include <endianess.h>
#include <print.h>

FAT_TYPE fat_determine_type(FAT_BPB* bpb);
const char* fat_type_to_string(FAT_TYPE t);

FatVolumeInformation* fat_get_device_context(FilesystemMount* fsmount);
void fat_set_device_context(FilesystemMount* fsmount, FatVolumeInformation* ctx);

const FilesystemOperations fat_ops = {
	fat_open,
	fat_close,
	fat_read,
	fat_seek
};

FilesystemMount* fat_mount(DiskDevice* device)
{
	printf("Probing for FAT on device %s\n", device->name);

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

	printf("FS on %s seems to be some kind of FAT\n", device->name);

	FatVolumeInformation* info = malloc(sizeof(FatVolumeInformation));
	memset(info, 0, sizeof(FatVolumeInformation));

	info->FatType = fat_determine_type(bpb);
	printf("It is %s\n", fat_type_to_string(info->FatType));

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

	printf("FatType: %s\n", fat_type_to_string(info->FatType));
	printf("BytesPerSector: %d\n", info->BytesPerSector);
	printf("SectorsPerCluster: %d\n", info->SectorsPerCluster);
	printf("FatSectorStart: %d\n", info->FatSectorStart);
	printf("ActiveFatSectorStart: %d\n", info->ActiveFatSectorStart);
	printf("NumberOfFats: %d\n", info->NumberOfFats);
	printf("SectorsPerFat: %d\n", info->SectorsPerFat);
	printf("RootDirSectorStart: %d\n", info->RootDirSectorStart);
	printf("RootDirSectors: %d\n", info->RootDirSectors);
	printf("RootDirStartCluster: %d\n", info->RootDirStartCluster);
	printf("DataSectorStart: %d\n", info->DataSectorStart);

	return fsmount;
}

void fat_umount(FilesystemMount* mount)
{
	//FIXME: todo
}


bool fat_close(FILE UNUSED(f))
{
	return false;
}

FILE fat_open(const char* UNUSED(path))
{
	return 0;
}

size_t fat_read(FILE UNUSED(f), void* UNUSED(buffer), size_t UNUSED(s))
{
	return 0;
}

void fat_seek(FILE UNUSED(f), SeekMode UNUSED(mode))
{

}

FAT_TYPE fat_determine_type(FAT_BPB* bpb)
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

const char* fat_type_to_string(FAT_TYPE t)
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

FatVolumeInformation* fat_get_device_context(FilesystemMount* fsmount)
{
	return (FatVolumeInformation*)fsmount->fsContext;
}

void fat_set_device_context(FilesystemMount* fsmount, FatVolumeInformation* ctx)
{
	fsmount->fsContext = (void*)ctx;
}
