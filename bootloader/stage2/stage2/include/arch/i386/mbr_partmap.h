#ifndef _GXLDR_ARCH_I386_MBR_PARTMAP_H_
#define _GXLDR_ARCH_I386_MBR_PARTMAP_H_

#include <stdint.h>

#include <disk.h>

struct DiskDevice_;

#define MBR_SIGNATURE               0xaa55

#define EXTENDED_DOS_PARTITION      0x05
#define EXTENDED_WINDOWS_PARTITION  0x0F

#define MAX_MBR_PARTITIONS          4

#define PMBR_GPT_PARTITION          0xEE
#define EFI_PARTITION               0xEF

#define MBR_SIZE                    512

#pragma pack(1)

typedef struct {
  uint8_t BootIndicator;
  uint8_t StartHead;
  uint8_t StartSector;
  uint8_t StartTrack;
  uint8_t OSIndicator;
  uint8_t EndHead;
  uint8_t EndSector;
  uint8_t EndTrack;
  uint32_t StartingLBA;
  uint32_t SizeInLBA;
} MBR_Partition;

///
/// MBR Partition Table
///
typedef struct {
  uint8_t                 BootStrapCode[440];
  uint8_t                 UniqueMbrSignature[4];
  uint8_t                 Unknown[2];
  MBR_Partition           Partition[MAX_MBR_PARTITIONS];
  uint16_t                Signature;
} MBR;

#pragma pack()

bool mbr_detectPartitions(struct DiskDevice_* device, AddDiskDeviceCallback cb);

//disk read callbacks
bool mbr_read_sectors(struct DiskDevice_* device, uint64_t startSector, uint32_t sectorCount, void* buffer);
bool mbr_get_disk_geometry(struct DiskDevice_* device, DiskGeometry* geometry);

#endif
