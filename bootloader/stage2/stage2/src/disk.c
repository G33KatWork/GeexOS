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
	list_for_each_entry_safe(dev1, dev2, &DeviceList, Link){
		list_del(&dev1->Link);

		Partition *p1, *p2;
		list_for_each_entry_safe(p1, p2, &dev1->partitions, Link){
			list_del(&p1->Link);
			free(p1);
		}

		free(dev1);
	}
}

void disk_printdevices()
{
	DiskDevice *dev;
	list_for_each_entry(dev, &DeviceList, Link){
		printf("%s: ", dev->name);
		Partition *part;
		list_for_each_entry(part, &dev->partitions, Link){
			printf("%s %D - %D    ", part->name, part->startLBA, part->endLBA);
		}
		printf("\n");
	}
}
