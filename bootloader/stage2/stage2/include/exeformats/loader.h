#ifndef _GEEXLDR_LOADER_H_
#define _GEEXLDR_LOADER_H_

#include <arch.h>
#include <list.h>

#define LOADER_IMAGETYPE_PE  1
#define LOADER_IMAGETYPE_ELF 2

typedef struct {
	char    Name[32];
	Address PhysicalBase;
	Address VirtualBase;
	Address OriginalBase;
	Address VirtualEntryPoint;
	size_t  SizeOfImage;
	struct list_head Link;
	int     ImageType;
	void*   Context;
} LoadedImage;

extern struct list_head loader_loadedImages;
extern const char* librarySearchPath;

void loader_printLoadedImages(void);
int loader_getLoadedImageCount(void);
bool loader_isImageLoaded(const char* name, LoadedImage** imageInfo);
LoadedImage* loader_getExecutable(void);

void loader_setLibrarySearchPath(const char* path);

#endif