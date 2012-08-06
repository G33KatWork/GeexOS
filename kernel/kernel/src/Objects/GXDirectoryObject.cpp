#include <kernel/Objects/GXDirectoryObject.h>

using namespace Objects;

void GXDirectoryObject::AddSubdirectory(GXDirectoryObject* dir)
{
	ASSERT(dir != NULL, "Directory to add may not be NULL");
	subdirectories.Append(dir);
}
