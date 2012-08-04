#ifndef _GEEXOS_OBJECT_DIRECTORY_H_
#define _GEEXOS_OBJECT_DIRECTORY_H_

#include <kernel/Objects/GXObjectType.h>

namespace Objects
{
	class GXDirectoryObject : public GXBaseObject<GXDirectoryObject>
	{
	public:
		GXDirectoryObject(GXObjectType<GXDirectoryObject>* Type, GXDirectoryObject* Parent, const char* Name)
			: Objects::GXBaseObject<Objects::GXDirectoryObject>::GXBaseObject(Type, Parent, Name){}

		virtual ~GXDirectoryObject() {}

		void foo();
	};
}

#endif
