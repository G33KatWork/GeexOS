#include <disk.h>
#include <arch.h>
#include <lib.h>
#include <print.h>

struct list_head DeviceList;

static void add_disk_device_callback(DiskDevice* device);

static void add_disk_device_callback(DiskDevice* device)
{
	list_add(&device->Link, &DeviceList);
}

void disk_init()
{
	INIT_LIST_HEAD(&DeviceList);
	arch_disksystem_setup(add_disk_device_callback);
}

void disk_destroy()
{
	DiskDevice *dev1, *dev2;
	list_for_each_entry_safe(dev1, dev2, &DeviceList, Link) {
		list_del(&dev1->Link);
		free(dev1);
	}
}

void disk_printdevices()
{
	DiskDevice *dev;
	list_for_each_entry(dev, &DeviceList, Link) {
		debug_printf("%s %D - %D    ", dev->name, dev->startLBA, dev->endLBA);
		debug_printf("\n");
	}
}

DiskDevice* disk_getDeviceByName(char* name)
{
	DiskDevice *dev;
	list_for_each_entry(dev, &DeviceList, Link) {
		if(strcmp(name, dev->name) == 0)
			return dev;
	}

	return NULL;
}
