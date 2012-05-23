#ifndef _GXLDR_FS_H_
#define _GXLDR_FS_H_

#include <types.h>
#include <disk.h>
#include <list.h>

typedef uint32_t FILE;

typedef enum {
	SEEK_CUR,
	SEEK_SET,
	SEEK_END
} SeekMode;

typedef struct {
	FILE (*open)(const char* path);
	bool (*close)(FILE f);
	size_t (*read)(FILE f, void* buffer, size_t s);
	void (*seek)(FILE f, SeekMode mode);
} FilesystemOperations;

struct FilesystemMount_ {
	const FilesystemOperations* fsops;
	DiskDevice* Device;
	void (*umount)(struct FilesystemMount_*);
	void* fsContext;
	struct list_head Link;
};
typedef struct FilesystemMount_ FilesystemMount;

void fs_init(void);

FILE open(const char* path);

#endif
