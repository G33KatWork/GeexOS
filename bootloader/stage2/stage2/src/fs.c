#include <fs.h>
#include <lib.h>
#include <print.h>
#include <disk.h>
#include <fs/fat.h>

typedef FilesystemMount* (*filesystem_mount)(DiskDevice*);
filesystem_mount filesystem_handlers[] = {
	fat_mount,
};

struct list_head FsMountList;

void fs_init()
{
	INIT_LIST_HEAD(&FsMountList);
}

FILE open(const char* path)
{
	char* filename = strchr(path, ')');
	if(!filename)
		return 0;

	filename++;

	char* devicename = (char*)malloc(filename - path + 1);
	memset(devicename, 0, filename - path + 1);
	memcpy(devicename, path, filename - path);

	printf("Opening disk device %s\n", devicename);

	//TODO: search in mounts... if not found, search for fs and create mount (see below)

	DiskDevice* device = disk_getDeviceByName(devicename);
	if(!device)
		return 0;

	FilesystemMount* fsmount = NULL;
	for(uint32_t i = 0; i < sizeof(filesystem_handlers) / sizeof(filesystem_mount); i++)
	{
		fsmount = filesystem_handlers[i](device);
		if(fsmount)
			break;
	}

	if(!fsmount)
	{
		printf("ERROR: disk device %s contains unknown filesystem\n", device->name);
		return 0;
	}

	list_add(&fsmount->Link, &FsMountList);

	return 1;
}
