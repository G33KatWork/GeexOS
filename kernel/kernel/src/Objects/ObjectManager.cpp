#include <kernel/Objects/ObjectManager.h>

using namespace Objects;

ObjectManager::ObjectManager()
{
	GXObjectType<GXDirectoryObject>* dirType = new GXObjectType<GXDirectoryObject>("ObjectDirectory");
    
    this->root = dirType->InstantiateObject(NULL, "");

    GXDirectoryObject* typeDir = dirType->InstantiateObject(NULL, "DefinedTypes");
    //TODO: typeDir->AddObject(dirType);
    this->root->AddSubdirectory(typeDir);
}

ObjectManager::~ObjectManager()
{
	delete this->root;
}

GXBaseObject* ObjectManager::InstantiateObjectByType(const char* Typename, const char* Name)
{
	//TODO
	return NULL;
}

GXBaseObject* ObjectManager::FindObjectByName(const char* Name)
{
	//TODO
	return NULL;
}
