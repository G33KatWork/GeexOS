#include <exeformats/loader.h>
#include <print.h>

LIST_HEAD(loader_loadedImages);
const char* librarySearchPath;

void loader_printLoadedImages()
{
	LoadedImage *img;
	list_for_each_entry(img, &loader_loadedImages, Link) {
		debug_printf("%s: Physical load address: 0x%X - Virtual base: 0x%X - Original base: 0x%X - Entrypoint: 0x%X - Imagesize: 0x%x - Type: %s\n",
			img->Name,
			img->PhysicalBase,
			img->VirtualBase,
			img->OriginalBase,
			img->VirtualEntryPoint,
			img->SizeOfImage,
			img->ImageType == LOADER_IMAGETYPE_PE ? "PE" : "ELF"
		);
	}
}

int loader_getLoadedImageCount()
{
	//Yeah! O(n) -_-
	int i = 0;
	LoadedImage *img;
	list_for_each_entry(img, &loader_loadedImages, Link) {
		i++;
	}

	return i;
}

bool loader_isImageLoaded(const char* name, LoadedImage** imageInfo)
{
	LoadedImage* image;
	list_for_each_entry(image, &loader_loadedImages, Link) {
		if(stricmp(name, image->Name) == 0)
		{
			if(imageInfo)
				*imageInfo = image;

			return true;
		}
	}

	return false;
}

void loader_setLibrarySearchPath(const char* path)
{
	//TODO: allow more than one path
	librarySearchPath = path;
}

LoadedImage* loader_getExecutable()
{
	//Return the tail of the list, this has to be the executable because it's first loaded
	if(!list_empty(&loader_loadedImages))
		return list_entry(loader_loadedImages.prev, LoadedImage, Link);

	return NULL;
}
