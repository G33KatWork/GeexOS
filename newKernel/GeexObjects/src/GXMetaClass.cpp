#include <Objects/GXMetaClass.h>

// GXMetaClassBase

GXMetaClassBase::GXMetaClassBase() {}
GXMetaClassBase::~GXMetaClassBase() {}

bool GXMetaClassBase::IsEqual(const GXMetaClassBase* other) const
{
    return this == other;
}

GXMetaClassBase* GXMetaClassBase::MetaCast(const GXMetaClass* to) const
{
    return to->CheckMetaCast(this);
}

/*GXMetaClassBase* GXMetaClassBase::MetaCast(const char* toMeta) const
{
    
}*/

GXMetaClassBase* GXMetaClassBase::SafeMetaCast(const GXMetaClassBase* anObject, const GXMetaClass* to)
{
    return (anObject) ? anObject->MetaCast(to) : NULL;
}

bool GXMetaClassBase::CheckTypeInst(const GXMetaClassBase* inst, const GXMetaClassBase* typeinst)
{
    //try to cast to given type and return true if successfull
    const GXMetaClass* to = GXTypeIDOfInstance(typeinst);
    return typeinst && inst && (inst->MetaCast(to) != NULL);
}


// Metadata class for GXMetaClass
class GXMetaClassMeta : public GXMetaClass 
{
public:
    GXMetaClassMeta();
    GXObject* Allocate() const;
};

GXMetaClassMeta::GXMetaClassMeta()
    : GXMetaClass("GXMetaClass", NULL, sizeof(GXMetaClass))
    { }
GXObject* GXMetaClassMeta::Allocate() const { return 0; }

//create static instance of meta metaclass
static GXMetaClassMeta sGXMetaClassMeta;



// GXMetaClass
//assign static pointer to metaclass of GXMetaClass
const GXMetaClass* const GXMetaClass::metaClass = &sGXMetaClassMeta;

const GXMetaClass* GXMetaClass::GetMetaClass() const { return &sGXMetaClassMeta; }

GXMetaClass::GXMetaClass() {}
GXMetaClass::~GXMetaClass() {}

GXMetaClass::GXMetaClass(const char* className_, const GXMetaClass* superClass_, size_t instanceSize_)
{
    this->className = className_;
    this->instanceSize = instanceSize_;
    this->superClass = superClass_;
}

const GXMetaClass* GXMetaClass::GetSuperClass() const
{
    return superClass;
}

const char* GXMetaClass::GetClassName() const
{
    return className;
}

GXMetaClassBase* GXMetaClass::CheckMetaCast(const GXMetaClassBase* check) const
{
    const GXMetaClass* to = this;
    const GXMetaClass* from;
    
    for(from = check->GetMetaClass(); ;from = from->superClass)
    {
        if(to == from)
            return (GXMetaClassBase*)check;
        
        if(!from->superClass)
            break;
    }
    
    return NULL;
}


// these functions all don't need to be implemented
// this class is only used as static instances inside GXObjects
int GXMetaClass::GetRetainCount() const { return 0; }

void GXMetaClass::Retain() const {}
void GXMetaClass::Release() const {}

void* GXMetaClass::operator new(size_t) { return NULL; }
void GXMetaClass::operator delete(void*, size_t) {}