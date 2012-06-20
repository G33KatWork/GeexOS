#ifndef _GEEXLDR_PELOADER_H_
#define _GEEXLDR_PELOADER_H_

#include <exeformats/pe.h>
#include <memory.h>
#include <list.h>

typedef struct {
	char 	Name[32];
	Address PhysicalBase;
	Address VirtualBase;
	Address VirtualEntryPoint;
	size_t 	SizeOfImage;
	struct list_head Link;
} LoadedImage;

extern struct list_head pe_loadedImages;

bool pe_loadFile(const char* filename, MemoryType memType, LoadedImage** imageInformation);
bool pe_resolveImports(LoadedImage* image);
bool pe_relocateImage(LoadedImage* image, int64_t bias);
void pe_printLoadedImages(void);
int pe_getLoadedImageCount(void);

bool pe_isImageLoaded(char* name, LoadedImage** imageInfo);
void pe_setLibrarySearchPath(const char* path);
PIMAGE_NT_HEADERS pe_getHeaders(void* base);
void* pe_getDirectoryEntry(LoadedImage* image, uint16_t directoryIndex, size_t* directorySize);

#endif
