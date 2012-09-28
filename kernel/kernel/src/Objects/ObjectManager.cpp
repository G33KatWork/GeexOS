#include <kernel/Objects/ObjectManager.h>
#include <string.h>

using namespace Objects;
using namespace Debug;

ObjectManager::ObjectManager()
{
	OBJ_SYS_DEBUG_MSG("Initializing object manager");

	GXObjectType<GXDirectoryObject>* dirType = new GXObjectType<GXDirectoryObject>("ObjectDirectory");
    
    this->root = dirType->InstantiateObject("Root");

    GXDirectoryObject* typeDir = dirType->InstantiateObject("DefinedTypes");
    typeDir->AddObject(dirType);
    this->root->AddSubdirectory(typeDir);
}

ObjectManager::~ObjectManager()
{
	delete this->root;
}

GXBaseObject* ObjectManager::InstantiateObjectByType(const char* Typename, const char* Name)
{
	OBJ_SYS_DEBUG_MSG("Instantiating object of type " << Typename << " with name " << Name);
	//TODO
	return NULL;
}

GXBaseObject* ObjectManager::FindObjectByName(const char* Name)
{
	OBJ_SYS_DEBUG_MSG("Searching for object with name " << Name);
	//TODO
	return NULL;
}

void ObjectManager::PrintLevel(Debug::BaseOutputDevice* c, GXDirectoryObject* dir, int level)
{
	for(int i = 0; i < 3*level; i++)
		*c << " ";

	*c << dir->GetName() << " (" << dir->GetTypeName() << ")" << endl;

	for(GXBaseObject* child = dir->GetSubObjects()->Head(); child != NULL; child = dir->GetSubObjects()->GetNext(child))
	{
		level++;
		
		if(strcmp(child->GetTypeName(), "ObjectDirectory") == 0)	
			this->PrintLevel(c, (GXDirectoryObject*)child, level);
		else
		{
			for(int i = 0; i < 3*level; i++)
				*c << " ";

			*c << child->GetName() << " (" << child->GetTypeName() << ")" << endl;
		}

		level--;
	}
}

void ObjectManager::DumpTree(Debug::BaseOutputDevice* c)
{
	this->PrintLevel(c, this->root, 0);
}
