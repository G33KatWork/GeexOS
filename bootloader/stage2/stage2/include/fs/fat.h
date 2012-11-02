#ifndef _GXLDR_FAT_H_
#define _GXLDR_FAT_H_

#include <stdint.h>
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

struct FatDirEntry_ {
	char Filename[11];
	uint8_t Attributes;
	uint8_t ReservedNt;
	uint8_t CreationTimeMs;
	uint16_t CreationTime;
	uint16_t CreationDate;
	uint16_t AccessDate;
	uint16_t ClusterHigh;
	uint16_t ModificationTime;
	uint16_t ModificationDate;
	uint16_t ClusterLow;
	uint32_t Size;
} __attribute__((packed));
typedef struct FatDirEntry_ FatDirEntry;

//FIXME: implement wchar support
struct FatLfnDirentry_
{
	uint8_t SequenceNumber;
	uint16_t Name0_4[5];
	uint8_t Attributes;
	uint8_t Reserved;
	uint8_t AliasChecksum;
	uint16_t Name5_10[6];
	uint16_t StartCluster;
	uint16_t Name11_12[2];
} __attribute__((packed));
typedef struct FatLfnDirentry_ FatLfnDirentry;

#define	ATTR_NORMAL		0x00
#define	ATTR_READONLY	0x01
#define	ATTR_HIDDEN		0x02
#define	ATTR_SYSTEM		0x04
#define	ATTR_VOLUMENAME	0x08
#define	ATTR_DIRECTORY	0x10
#define	ATTR_ARCHIVE	0x20
#define ATTR_LONG_NAME	(ATTR_READONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUMENAME)


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

typedef struct {
	uint8_t Attributes;
	uint32_t Filesize;
	uint32_t* FATChain;
} FatFileInfo;

FilesystemMount* fat_mount(DiskDevice* device);
void fat_umount(FilesystemMount* mount);

bool fat_close(FILE* f);
FILE* fat_open(const char* path, FilesystemMount* mount);
size_t fat_read(FILE* f, void* buffer, size_t s);
void fat_seek(FILE* f, long offset, SeekMode mode);

bool fat_findFile(FilesystemMount* mount, const char* filename, FatFileInfo* fileinfo);
bool fat_searchDirectoryForFile(FilesystemMount* mount, void* directoryContents, size_t directorySize, const char* filename, FatFileInfo* fileinfo);
void fat_parseShortFilename(FatDirEntry* entry, char* buffer);
void* fat_getDirectoryContents(FilesystemMount* mount, uint32_t startCluster, size_t* directorySize, bool rootDirectory);
bool fat_readClusterChain(FilesystemMount* mount, uint32_t startCluster, uint32_t maxClusters, void* buffer);
bool fat_readPartialCluster(FilesystemMount* mount, uint32_t clusterNumber, size_t offset, size_t length, void* buffer);
bool fat_getFatEntry(FilesystemMount* mount, uint32_t cluster, uint32_t* result);
uint32_t fat_countClustersInChain(FilesystemMount* mount, uint32_t startCluster);
uint32_t* fat_getFatChain(FilesystemMount* mount, uint32_t startCluster);

#endif
