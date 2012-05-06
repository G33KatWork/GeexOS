#ifndef _GXLDR_ARCH_I386_BIOSDISK_H_
#define _GXLDR_ARCH_I386_BIOSDISK_H_

#include <types.h>

struct INT13_LBA_Packet {
	uint8_t Size;
	uint8_t reserved;
	uint16_t NumSectors;
	uint16_t BufferOffset;
	uint16_t BufferSegment;
	uint32_t LBAStartSector;
	uint32_t LBAStartSectorHigh;
} __attribute__((packed));

//TODO: move to general disk read module:
typedef struct {
	uint32_t Heads;
	uint32_t Cylinders;
	uint32_t Sectors;
	uint32_t BytesPerSector;
} DiskGeometry;

struct _ExtendedDiskParameters {
	uint16_t Size;
	uint16_t Flags;
	uint32_t Cylinders;
	uint32_t Heads;
	uint32_t Sectors;
	uint16_t BytesPerSector;
	uint32_t EDDPTR;
} __attribute__((packed));
typedef struct _ExtendedDiskParameters ExtendedDiskParameters;

typedef enum {
	DISKTYPE_NOT_PRESENT = 0,
	DISKTYPE_FLOPPY = 1,
	DISKTYPE_FLOPPY_CHANGEDETECT = 2,
	DISKTYPE_FIXED = 3,
	DISKTYPE_DETECTION_ERROR = -1
} DiskType;

void biosdisk_i386_initialize(void);

bool bootdisk_i386_reset_controller(uint8_t driveNumber);
bool biosdisk_i386_extendedmode_supported(uint8_t driveNumber);
bool biosdisk_i386_get_disk_geometry(uint8_t driveNumber, DiskGeometry* geometry);
bool biosdisk_i386_get_extended_drive_parameters(uint8_t driveNumber, ExtendedDiskParameters* parameters);
DiskType biosdisk_i386_get_disk_type(uint8_t driveNumber);

bool biosdisk_i386_read_sectors(uint8_t driveNumber, uint64_t startSector, uint32_t sectorCount, void* buffer);

bool biosdisk_i386_read_lba(uint8_t driveNumber, uint64_t startSector, uint32_t sectorCount, void* buffer);
bool biosdisk_i386_read_chs(uint8_t driveNumber, uint64_t startSector, uint32_t sectorCount, void* buffer);

#endif
