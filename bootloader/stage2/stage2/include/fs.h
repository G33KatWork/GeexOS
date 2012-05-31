#ifndef _GXLDR_FS_H_
#define _GXLDR_FS_H_

#include <types.h>
#include <disk.h>
#include <list.h>

typedef enum {
	SEEK_CUR,
	SEEK_SET,
	SEEK_END
} SeekMode;

//the file-pointer structure
struct FILE_ {
	struct FilesystemMount_* mount;
	void* fsContext;				//fs-driver specific
	uint32_t filePointer;
};
typedef struct FILE_ FILE;

//operations which need to be implemented by the FS-Driver
typedef struct {
	FILE* (*open)(const char* path, struct FilesystemMount_* mount);
	bool (*close)(FILE* f);
	size_t (*read)(FILE* f, void* buffer, size_t s);
	void (*seek)(FILE* f, long offset, SeekMode mode);
} FilesystemOperations;

//metadata for a mounted filesystem
struct FilesystemMount_ {
	const FilesystemOperations* fsops;
	DiskDevice* Device;
	void (*umount)(struct FilesystemMount_*);
	void* fsContext;				//fs-driver specific
	struct list_head Link;
};
typedef struct FilesystemMount_ FilesystemMount;

void fs_init(void);

//file functions
FILE* open(const char* path);
bool close(FILE* f);
void seek(FILE* f, long offset, SeekMode m);
size_t read(FILE* f, void* buffer, size_t s);

//path helper functions
uint32_t fs_getNumPathParts(const char* path);
void fs_getFirstElementFromPath(const char* path, char* buffer);

#endif
