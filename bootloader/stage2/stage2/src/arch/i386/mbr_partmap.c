#include <arch/i386/mbr_partmap.h>
#include <disk.h>
#include <endianess.h>
#include <print.h>

static bool mbr_isValid(MBR* mbr, uint64_t totalSectors)
{
	if(mbr->Signature != MBR_SIGNATURE)
		return false;

	bool valid = false;
	
	for(int i = 0; i < MAX_MBR_PARTITIONS; i++)
	{
		if(mbr->Partition[i].OSIndicator == 0 || LE_TO_HOST32(mbr->Partition[i].SizeInLBA) == 0)
			continue;

		valid = true;
		uint32_t startLBA = LE_TO_HOST32(mbr->Partition[i].StartingLBA);
		uint32_t endLBA = startLBA + LE_TO_HOST32(mbr->Partition[i].SizeInLBA) - 1;
		
		if(endLBA >= totalSectors)
			return false;
			

		for(int j = i + 1; j < MAX_MBR_PARTITIONS; j++)
		{
			if(mbr->Partition[j].OSIndicator == 0 || LE_TO_HOST32(mbr->Partition[j].SizeInLBA) == 0)
				continue;

			uint32_t newEnd = LE_TO_HOST32(mbr->Partition[j].StartingLBA) + LE_TO_HOST32(mbr->Partition[j].SizeInLBA);
			if(newEnd >= startLBA && LE_TO_HOST32(mbr->Partition[j].StartingLBA) <= endLBA)
				return false;
		}
	}

	printf("Disk seems to have a valid mbr\n");
	return valid;
}

bool mbr_detectPartitions(struct DiskDevice_* device, AddDiskDeviceCallback cb)
{
	DiskGeometry geometry;
	if(!device->get_disk_geometry(device, &geometry))
		return false;

	uint8_t firstSector[geometry.BytesPerSector];
	if(!device->read_sectors(device, 0, 1, firstSector))
		return false;

	if(!mbr_isValid((MBR*)firstSector, geometry.TotalLBASectors))
		return false;

	MBR* mbr = (MBR*)firstSector;

	for(int i = 0; i < MAX_MBR_PARTITIONS; i++)
	{
		if(mbr->Partition[i].OSIndicator == 0 || LE_TO_HOST32(mbr->Partition[i].SizeInLBA) == 0)
			continue;

		if(mbr->Partition[i].OSIndicator == PMBR_GPT_PARTITION)
			continue;

		//TODO: handle extended partitions
		DiskDevice* part = (DiskDevice*)malloc(sizeof(DiskDevice));
		memset(part, 0, sizeof(DiskDevice));
		part->parentDevice = device;
		part->startLBA = LE_TO_HOST32(mbr->Partition[i].StartingLBA);
		part->endLBA = LE_TO_HOST32(mbr->Partition[i].StartingLBA) + LE_TO_HOST32(mbr->Partition[i].SizeInLBA) - 1;
		part->diskNumber = i;
		part->type = DEVICETYPE_PARTITION;
		part->read_sectors = mbr_read_sectors;
		part->get_disk_geometry = mbr_get_disk_geometry;
		snprintf(part->name, sizeof(part->name), "hd(%d,%d)", device->diskNumber, i);

		cb(part);
	}

	return true;
}

bool mbr_read_sectors(DiskDevice* device, uint64_t startSector, uint32_t sectorCount, void* buffer)
{
	if(device->type != DEVICETYPE_PARTITION)
		return false;

	if(!device->parentDevice)
		return false;

	//FIXME: check if read is in bounds of this partition

	return device->parentDevice->read_sectors(device->parentDevice, startSector + device->startLBA, sectorCount, buffer);
}

bool mbr_get_disk_geometry(DiskDevice* device, DiskGeometry* geometry)
{
	return device->parentDevice->get_disk_geometry(device->parentDevice, geometry);
}
