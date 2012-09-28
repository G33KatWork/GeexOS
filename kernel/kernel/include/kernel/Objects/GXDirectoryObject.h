#ifndef _GEEXOS_OBJECT_DIRECTORY_H_
#define _GEEXOS_OBJECT_DIRECTORY_H_

#include <kernel/Objects/GXObjectType.h>
#include <kernel/DataStructures/DoublyLinkedList.h>

namespace Objects
{
	class GXDirectoryObject : public GXTypedObject<GXDirectoryObject>
	{
	private:
		DataStructures::DoublyLinkedList<GXBaseObject> subobjects;

	public:
		GXDirectoryObject(GXObjectType<GXDirectoryObject>* Type, const char* Name)
			: Objects::GXTypedObject<Objects::GXDirectoryObject>::GXTypedObject(Type, Name) {}
		virtual ~GXDirectoryObject() {}

		void AddSubdirectory(GXDirectoryObject* dir);
		void AddObject(GXBaseObject* obj);

		const DataStructures::DoublyLinkedList<GXBaseObject>* GetSubObjects() const { return &this->subobjects; }
	};
}

#endif
