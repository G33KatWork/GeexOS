#include <kernel/Objects/GXDirectoryObject.h>

using namespace Objects;

void GXDirectoryObject::AddSubdirectory(GXDirectoryObject* dir)
{
	OBJ_SYS_DEBUG_MSG("Adding subdirectory object with name " << dir->GetName() <<  " to directory " << this->GetName());
	
	ASSERT(dir != NULL, "Directory to add may not be NULL");
	subobjects.Append(dir);
}

void GXDirectoryObject::AddObject(GXBaseObject* obj)
{
	OBJ_SYS_DEBUG_MSG("Adding object with name " << obj->GetName() << " of type " << obj->GetTypeName() << " to directory " << this->GetName());

	ASSERT(obj != NULL, "Object to add may not be NULL");
	subobjects.Append(obj);
}
