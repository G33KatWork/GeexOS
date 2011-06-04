#include <Objects/GXObject.h>

//Metadata class foo
//Move into macro later?

GXObject::MetaClass GXObject::gMetaClass;
const GXMetaClass * const GXObject::metaClass = &GXObject::gMetaClass;
const GXMetaClass * const GXObject::superClass = 0;

const GXMetaClass * GXObject::GetMetaClass() const
    { return &gMetaClass; }
GXObject *GXObject::MetaClass::Allocate() const { return 0; }

//Metaclass constructor
GXObject::MetaClass::MetaClass()
    : GXMetaClass("GXObject", GXObject::superClass, sizeof(GXObject))
    { }


GXObject::GXObject() { retainCount = 1; }
GXObject::GXObject(const GXMetaClass*) { retainCount = 1; }
GXObject::~GXObject() {}

int GXObject::GetRetainCount() const
{
    return retainCount;
}

void GXObject::Retain() const
{
    //FIXME: Make threadsafe
    retainCount++;
}

void GXObject::Release() const
{
    //FIXME: Make threadsafe
    retainCount--;
    //if(retainCount == 0xffff)
    if(retainCount == 0)
        delete this;
}

void GXObject::Free() {}
bool GXObject::Initialize() { return true; }

void GXObject::operator delete(void* obj, size_t size)
{
    //OBJ_SYS_DEBUG_MGS("Deleting object at " << hex << (Address)obj << " of size " << dec << size);
    //free(obj);
}

void* GXObject::operator new(size_t size)
{
    //OBJ_SYS_DEBUG_MGS("Allocating object of size " << dec << size);
    //return malloc(size);
}
