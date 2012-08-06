#ifndef _GEEXOS_OBJECT_DIRECTORY_H_
#define _GEEXOS_OBJECT_DIRECTORY_H_

#include <kernel/Objects/GXObjectType.h>
#include <kernel/DataStructures/DoublyLinkedList.h>

namespace Objects
{
	class GXDirectoryObject : public GXTypedObject<GXDirectoryObject>
	{
	private:
		DataStructures::DoublyLinkedList<GXBaseObject> subdirectories;

	public:
		GXDirectoryObject(GXObjectType<GXDirectoryObject>* Type, GXDirectoryObject* Parent, const char* Name)
			: Objects::GXTypedObject<Objects::GXDirectoryObject>::GXTypedObject(Type, Parent, Name) {}
		virtual ~GXDirectoryObject() {}

		void AddSubdirectory(GXDirectoryObject* dir);
	};
}

#endif
