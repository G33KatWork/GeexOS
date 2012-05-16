#ifndef _GXLDR_DISK_H_
#define _GXLDR_DISK_H_

#include <types.h>
#include <list.h>

typedef struct {
	uint32_t Heads;
	uint32_t Cylinders;
	uint32_t SectorsPerTrack;
	uint64_t TotalLBASectors;
	uint32_t BytesPerSector;
} DiskGeometry;

typedef enum {
	DEVICETYPE_FLOPPY,
	DEVICETYPE_HDD,
	DEVICETYPE_CDROM
} DeviceType;

struct DiskDevice_
{
	char name[32];
	DeviceType type;
	int diskNumber;
	bool (*read_sectors)(struct DiskDevice_* device, uint64_t startSector, uint32_t sectorCount, void* buffer);
	bool (*get_disk_geometry)(struct DiskDevice_* device, DiskGeometry* geometry);
	struct list_head partitions;
	struct list_head Link;
};
typedef struct DiskDevice_ DiskDevice;

struct Partition_ {
	char name[32];
	int partitionNumber;
	uint64_t startLBA;
	uint64_t endLBA;
	DiskDevice* disk;
	struct list_head Link;
};
typedef struct Partition_ Partition;

typedef void (*AddDiskDeviceCallback)(DiskDevice* device);

void disk_init(void);
void disk_destroy(void);
void disk_printdevices(void);

#endif
