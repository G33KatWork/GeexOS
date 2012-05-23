#ifndef _GXLDR_FAT_H_
#define _GXLDR_FAT_H_

#include <types.h>
#include <fs.h>

struct FAT_BPB_ {
	uint8_t  JmpBoot[3];
	uint8_t  OemName[8];
	uint16_t BytesPerSector;
	uint8_t  SectorsPerCluster;
	uint16_t ReservedSectors;
	uint8_t  NumberOfFats;
	uint16_t RootDirEntries;
	uint16_t TotalSectors;
	uint8_t  MediaType;
	uint16_t SectorsPerFat;
	uint16_t SectorsPerTrack;
	uint16_t NumberOfHeads;
	uint32_t HiddenSectors;
	uint32_t TotalSectorsBig;

	union {
		struct {
			uint8_t  DriveNumber;
			uint8_t  Reserved1;
			uint8_t  BootSignature;
			uint32_t VolumeSerialNumber;
			uint8_t  VolumeLabel[11];
			uint8_t  FilesystemType[8];
		} __attribute__ ((packed)) FAT12_16;

		struct {
			uint32_t SectorsPerFatBig;
			uint16_t ExtendedFlags;
			uint16_t FilesystemVersion;
			uint32_t RootDirStartCluster;
			uint16_t FsInfo;
			uint16_t BackupBootSector;
			uint8_t  Reserved[12];
			uint8_t  DriveNumber;
			uint8_t  Reserved1;
			uint8_t  BootSignature;
			uint32_t VolumeSerialNumber;
			uint8_t  VolumeLabel[11];
			uint8_t  FilesystemType[8];
		} __attribute__ ((packed)) FAT32;

	} __attribute__ ((packed)) FAT;

	uint8_t pad[422];
} __attribute__ ((packed));
typedef struct FAT_BPB_ FAT_BPB;

typedef enum {
	FAT12,
	FAT16,
	FAT32
} FAT_TYPE;

typedef struct {
	uint32_t BytesPerSector;
	uint32_t SectorsPerCluster;
	uint32_t FatSectorStart;
	uint32_t ActiveFatSectorStart;
	uint32_t NumberOfFats;
	uint32_t SectorsPerFat;
	uint32_t RootDirSectorStart;
	uint32_t RootDirSectors;
	uint32_t RootDirStartCluster;
	uint32_t DataSectorStart;
	FAT_TYPE FatType;
} FatVolumeInformation;

FilesystemMount* fat_mount(DiskDevice* device);
void fat_umount(FilesystemMount* mount);

bool fat_close(FILE f);
FILE fat_open(const char* path);
size_t fat_read(FILE f, void* buffer, size_t s);
void fat_seek(FILE f, SeekMode mode);

#endif
