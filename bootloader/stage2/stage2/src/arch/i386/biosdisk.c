#include <arch/i386/biosdisk.h>
#include <arch/i386/bioscall.h>
#include <arch/i386/mbr_partmap.h>

#include <lib.h>
#include <arch.h>
#include <debug.h>
#include <print.h>

typedef bool (*detect_partitions)(DiskDevice*, AddDiskDeviceCallback);
detect_partitions partmap_handlers[] = {
	mbr_detectPartitions,
};

void biosdisk_i386_initialize(AddDiskDeviceCallback cb)
{
	//detect floppies
	for(int floppy = 0; floppy < 2; floppy++) {
		biosdisk_i386_reset_controller(floppy);

		DiskType t = biosdisk_i386_get_disk_type(floppy);
		if(t == DISKTYPE_FLOPPY || t == DISKTYPE_FLOPPY_CHANGEDETECT)
		{
			DiskDevice* dev = (DiskDevice*)malloc(sizeof(DiskDevice));
			memset(dev, 0, sizeof(DiskDevice));
			snprintf(dev->name, sizeof(dev->name), "fd(%d)", floppy);
			dev->type = DEVICETYPE_FLOPPY;
			dev->diskNumber = floppy;
			dev->read_sectors = disk_interface_read_sectors;
			dev->get_disk_geometry = disk_interface_get_disk_geometry;

			debug_printf("Detected floppy at drive num 0x%x with type %x\n", floppy, t);
			cb(dev);
		}
	}

	//detect hdds
	for(int hdd = 0x80; hdd < 0x90; hdd++) {
		DiskType t = biosdisk_i386_get_disk_type(hdd);
		if(t == DISKTYPE_FIXED)
		{
			DiskDevice* dev = (DiskDevice*)malloc(sizeof(DiskDevice));
			memset(dev, 0, sizeof(DiskDevice));
			snprintf(dev->name, sizeof(dev->name), "hd(%d)", hdd - 0x80);
			dev->type = DEVICETYPE_HDD;
			dev->diskNumber = hdd - 0x80;
			dev->read_sectors = disk_interface_read_sectors;
			dev->get_disk_geometry = disk_interface_get_disk_geometry;

			for(uint32_t i = 0; i < sizeof(partmap_handlers) / sizeof(detect_partitions); i++)
			{
				if(partmap_handlers[i](dev, cb))
					break;
			}

			debug_printf("Detected hdd at drive num 0x%x with type %x\n", hdd, t);
			cb(dev);
		}
	}

	//TODO: detect cdroms
}

bool biosdisk_i386_reset_controller(uint8_t driveNumber)
{
	bios_int_registers regs;
	memset(&regs, 0, sizeof(bios_int_registers));

	regs.eax = 0x0000;
	regs.edx = driveNumber;
	callInt(0x13, &regs);

	return !EFLAGS_IS_SET(regs.flags, EFLAGS_CARRY);
}

bool biosdisk_i386_extendedmode_supported(uint8_t driveNumber)
{
	bios_int_registers regs;
	memset(&regs, 0, sizeof(bios_int_registers));

	//query for INT13h LBA extensions
	regs.eax = 0x4100;
	regs.ebx = 0x55AA;
	regs.edx = driveNumber;
	callInt(0x13, &regs);

	//If carry flag is set, the extensions are not supported
	return !EFLAGS_IS_SET(regs.flags, EFLAGS_CARRY);
}

bool biosdisk_i386_read_sectors(uint8_t driveNumber, uint64_t startSector, uint32_t sectorCount, void* buffer)
{
	//if((Address)buffer > 0x100000)
	//	arch_panic("Can't perform a disk read into a buffer above 1M. Buffer is at 0x%x\n", buffer);

	if((Address)buffer > 0x100000)
	{
		//FIXME: HACKHACK do this in a loop to read more than 24k
		//read 24k into the bios buffer in low memory and then copy it over

		DiskGeometry geometry;
		if(!biosdisk_i386_get_disk_geometry(driveNumber, &geometry))
			return false;

		if(sectorCount * geometry.BytesPerSector > BIOSCALLBUFFER_SIZE)
			arch_panic("Can't perform a disk read into a buffer above 1M with more than 24K at once");

		bool ret;
		if(biosdisk_i386_extendedmode_supported(driveNumber))
			ret = biosdisk_i386_read_lba(driveNumber, startSector, sectorCount, (void*)BIOSCALLBUFFER);
		else
			ret = biosdisk_i386_read_chs(driveNumber, startSector, sectorCount, (void*)BIOSCALLBUFFER);

		memcpy(buffer, (void*)BIOSCALLBUFFER, sectorCount * geometry.BytesPerSector);
		return ret;
	}
	else
	{
		if(biosdisk_i386_extendedmode_supported(driveNumber))
			return biosdisk_i386_read_lba(driveNumber, startSector, sectorCount, buffer);
		else
			return biosdisk_i386_read_chs(driveNumber, startSector, sectorCount, buffer);
	}
}

bool biosdisk_i386_get_disk_geometry(uint8_t driveNumber, DiskGeometry* geometry)
{
	if(!geometry)
		arch_panic("Pointer to disk geometry struct may not be NULL!");

	ExtendedDiskParameters extParams;
	if(biosdisk_i386_get_extended_drive_parameters(driveNumber, &extParams))
	{
		geometry->Cylinders = extParams.Cylinders;
		geometry->Heads = extParams.Heads;
		geometry->SectorsPerTrack = extParams.Sectors;
		geometry->TotalLBASectors = extParams.TotalLBASectors;
		geometry->BytesPerSector = extParams.BytesPerSector;

		return true;
	}
	else
	{
		bios_int_registers regs;
		memset(&regs, 0, sizeof(bios_int_registers));

		//query for INT13h LBA extensions
		regs.eax = 0x0800;
		regs.edx = driveNumber;
		callInt(0x13, &regs);

		if(EFLAGS_IS_SET(regs.flags, EFLAGS_CARRY))
			return false;

		uint8_t c = ((regs.ecx & 0xFF) & 0xC0) << 2;
		c += ((regs.ecx & 0xFF00) >> 8);
		c++;
		geometry->Cylinders = c;
		geometry->Heads = ((regs.ebx & 0xFF00) >> 8) + 1;
		geometry->SectorsPerTrack = (regs.ecx & 0xFF) & 0x3F;
		geometry->TotalLBASectors = geometry->Cylinders * geometry->Heads * geometry->SectorsPerTrack;
		geometry->BytesPerSector = 512;

		return true;
	}
}

bool biosdisk_i386_get_extended_drive_parameters(uint8_t driveNumber, ExtendedDiskParameters* parameters)
{
	if(!parameters)
		arch_panic("Pointer to extended disk parameters struct may not be NULL!");

	if(!biosdisk_i386_extendedmode_supported(driveNumber))
		return false;

	ExtendedDiskParameters* parms = (ExtendedDiskParameters*)BIOSCALLBUFFER;
	memset(parms, 0, sizeof(ExtendedDiskParameters));
	parms->Size = sizeof(ExtendedDiskParameters);

	bios_int_registers regs;
	memset(&regs, 0, sizeof(bios_int_registers));

	regs.eax = 0x4800;
	regs.edx = driveNumber;
	regs.ds = BIOSCALLBUF_SEGMENT;
	regs.esi = BIOSCALLBUF_OFFSET;
	callInt(0x13, &regs);

	if(EFLAGS_IS_SET(regs.flags, EFLAGS_CARRY))
		return false;

	memcpy(parameters, (void*)BIOSCALLBUFFER, sizeof(ExtendedDiskParameters));
	return true;
}

DiskType biosdisk_i386_get_disk_type(uint8_t driveNumber)
{
	bios_int_registers regs;
	memset(&regs, 0, sizeof(bios_int_registers));

	//query for INT13h LBA extensions
	regs.eax = 0x1500;
	regs.edx = driveNumber;
	callInt(0x13, &regs);

	if EFLAGS_IS_SET(regs.flags, EFLAGS_CARRY)
		return DISKTYPE_DETECTION_ERROR;

	return (DiskType)((regs.eax >> 8) & 0xFF);
}

bool biosdisk_i386_read_lba(uint8_t driveNumber, uint64_t startSector, uint32_t sectorCount, void* buffer)
{
	//Try 3 times
	for(int i = 0; i < 3; i++)
	{
		struct INT13_LBA_Packet* packet = (struct INT13_LBA_Packet*)BIOSCALLBUFFER;
		memset(packet, 0, sizeof(struct INT13_LBA_Packet));

		packet->Size = sizeof(struct INT13_LBA_Packet);
		packet->NumSectors = sectorCount;
		packet->LBAStartSector = (uint32_t)(startSector & 0xFFFFFFFFL);
		packet->LBAStartSectorHigh = (uint32_t)((startSector >> 32) & 0xFFFFFFFFL);
		packet->BufferSegment = ((Address)buffer) >> 4;
		packet->BufferOffset = ((Address)buffer) & 0x0F;

		bios_int_registers regs;
		memset(&regs, 0, sizeof(bios_int_registers));

		regs.eax = 0x4200;
		regs.edx = driveNumber;
		regs.ds = BIOSCALLBUF_SEGMENT;
		regs.esi = BIOSCALLBUF_OFFSET;
		callInt(0x13, &regs);

		if(!EFLAGS_IS_SET(regs.flags, EFLAGS_CARRY))
			return true;

		debug_printf("WARN: LBA disk read failed. Retrying...\n");
		biosdisk_i386_reset_controller(driveNumber);
	}
	
	debug_printf("ERR: LBA disk read failed.\n");
	return false;
}

bool biosdisk_i386_read_chs(uint8_t driveNumber, uint64_t startSector, uint32_t sectorCount, void* buffer)
{
	DiskGeometry g;
    if(!biosdisk_i386_get_disk_geometry(driveNumber, &g))
    	return false;

    //HACKHACK - dont't want to implement 64 bit arithmetic helper...
    uint32_t startSector32 = (uint32_t)(startSector & 0xFFFFFFFF);

    uint8_t numberOfSectorsToRead = 0;

   	while(sectorCount)
   	{
   		uint8_t physicalSector = 1 + (uint8_t)(startSector32 % g.SectorsPerTrack);
   		uint8_t physicalHead = (uint8_t)((startSector32 / g.SectorsPerTrack) % g.Heads);
   		uint32_t physicalTrack = (uint32_t)((startSector32 / g.SectorsPerTrack) / g.Heads);

   		//printf("CHS Read from \nCylinder %x\nHead %x\nSector %x\n", physicalTrack, physicalHead, physicalSector);

   		if(physicalSector > 1)
   		{
   			if(sectorCount >= (uint32_t)(g.SectorsPerTrack - (physicalSector - 1)))
   				numberOfSectorsToRead = (g.SectorsPerTrack) - (physicalSector - 1);
   			else
   				numberOfSectorsToRead = sectorCount;
   		}
   		else
   		{
   			if(sectorCount >= g.SectorsPerTrack)
   				numberOfSectorsToRead = g.SectorsPerTrack;
   			else
   				numberOfSectorsToRead = sectorCount;
   		}

   		//printf("Reading %x sectors\n", numberOfSectorsToRead);

   		//bounds checking
   		if (
   			physicalHead >= g.Heads ||
   			physicalTrack >= g.Cylinders ||
   			(numberOfSectorsToRead + physicalSector) > (g.SectorsPerTrack + 1) ||
   			physicalSector > g.SectorsPerTrack
   		)
   		{
   			debug_printf("ERR: CHS Disk read parameters exceed geometry limits\n");
   			return false;
   		}

   		//perform read
   		int i = 0;
   		for(i = 0; i < 3; i++)
		{
			bios_int_registers regs;
			memset(&regs, 0, sizeof(bios_int_registers));

			regs.eax = (0x02 << 8) | numberOfSectorsToRead;
			regs.ecx = (((physicalTrack & 0xFF)) << 8) | (uint8_t)(physicalSector + ((physicalTrack & 0x300) >> 2));
			regs.edx = (physicalHead << 8) | driveNumber;
			regs.es = (uint16_t)(((Address)buffer) >> 4);
			regs.ebx = (uint16_t)(((Address)buffer) & 0x0F);
			callInt(0x13, &regs);

			if(!EFLAGS_IS_SET(regs.flags, EFLAGS_CARRY))
				break;

			debug_printf("WARN: LBA disk read failed. Retrying...\n");
			biosdisk_i386_reset_controller(driveNumber);
			continue;
		}

		if(i >= 3)
		{
			debug_printf("ERR: CHS disk read failed.\n");
			return false;
		}

		buffer = (void*)(((Address)buffer) + numberOfSectorsToRead * g.BytesPerSector);
		sectorCount -= numberOfSectorsToRead;
		startSector32 += numberOfSectorsToRead;
   	}

   	return true;
}

bool disk_interface_read_sectors(struct DiskDevice_* device, uint64_t startSector, uint32_t sectorCount, void* buffer)
{
	int diskNumber = device->diskNumber;
	if(device->type == DEVICETYPE_HDD)
		diskNumber += 0x80;

	return biosdisk_i386_read_sectors(diskNumber, startSector, sectorCount, buffer);
}

bool disk_interface_get_disk_geometry(struct DiskDevice_* device, DiskGeometry* geometry)
{
	int diskNumber = device->diskNumber;
	if(device->type == DEVICETYPE_HDD)
		diskNumber += 0x80;

	return biosdisk_i386_get_disk_geometry(diskNumber, geometry);
}
