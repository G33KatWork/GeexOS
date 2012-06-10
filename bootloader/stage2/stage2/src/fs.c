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

void fs_shutdown()
{
	FilesystemMount *mount;
	list_for_each_entry(mount, &FsMountList, Link) {
		mount->umount(mount);
	}
}

FILE* open(const char* path)
{
	printf("FS: opening %s\n", path);

	char* filename = strchr(path, ')');
	if(!filename)
		return 0;

	filename++;

	char* devicename = (char*)malloc(filename - path + 1);
	memset(devicename, 0, filename - path + 1);
	memcpy(devicename, path, filename - path);

	//skip leading slash or backslash
	if(*filename == '\\' || *filename == '/')
		filename++;

	printf("Opening file %s disk device %s\n", filename, devicename);

	FilesystemMount *mount;
	list_for_each_entry(mount, &FsMountList, Link) {
		if(strcmp(devicename, mount->Device->name) == 0)
		{
			printf("FS on device %s already mounted\n", devicename);
			return mount->fsops->open(filename, mount);
		}
	}

	//Device not mounted yet, try to do that
	DiskDevice* device = disk_getDeviceByName(devicename);
	free(devicename);
	if(!device)
		return NULL;

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

	return fsmount->fsops->open(filename, fsmount);
}

bool close(FILE* f)
{
	if(f)
		return f->mount->fsops->close(f);

	return false;
}

void seek(FILE* f, long offset, SeekMode m)
{
	if(f)
		f->mount->fsops->seek(f, offset, m);
}

size_t read(FILE* f, void* buffer, size_t s)
{
	if(f)
		return f->mount->fsops->read(f, buffer, s);

	return 0;
}

uint32_t fs_getNumPathParts(const char* path)
{
	size_t i;
	uint32_t num;

	for(i = 0, num = 0; i < strlen(path); i++)
	{
		if(path[i] == '\\' || path[i] == '/')
			num++;
	}
	num++;

	return num;
}

void fs_getFirstElementFromPath(const char* path, char* buffer)
{
	size_t i;
	for(i = 0; i < strlen(path); i++)
	{
		if(path[i] == '\\' || path[i] == '/')
			break;
		else
			buffer[i] = path[i];
	}

	buffer[i] = 0;
}

const char* fs_getFilename(const char* path)
{
	const char *filename = strrchr(path, '/');
    
    if(filename == NULL)
        filename = path;
    else
        filename++;

    return filename;
}

void fs_concatPath(char* first, const char* second, size_t buflen)
{
	if(!first || !second)
		return;

	char lastChar = first[strlen(first)-1];

	if(lastChar != '/')
		strncat(first, "/", buflen - strlen(first) - 1);

	strncat(first, second, buflen - strlen(first) - 1);
}
