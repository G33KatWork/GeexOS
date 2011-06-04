#include <iostream>
#include <Objects/GXObject.h>

using namespace std;

class MyTestObject : public GXObject
{
GXDeclareDefaultMetadata(MyTestObject)

protected:
    int a;

public:
    int GetA() { return a; }
    void SetA(int newA) { a = newA; }
    
    virtual bool Initialize();
};

class MyTestObject2 : public GXObject
{
GXDeclareDefaultMetadata(MyTestObject2)

protected:
    int a;

public:
    int GetA() { return a; }
    void SetA(int newA) { a = newA; }
    
    virtual bool Initialize();
};

class MyTestObject3 : public MyTestObject
{
GXDeclareDefaultMetadata(MyTestObject3)

public:
    virtual bool Initialize();
};


void DumpObject(GXObject* o)
{
    cout << "Object is at " << hex << o << endl;
    cout << "Retain count is " << dec << o->GetRetainCount() << endl;
    cout << "Object is of class " << GXTypeIDOfInstance(o)->GetClassName() << endl;
    cout << "And derived from superclass " << GXTypeIDOfInstance(o)->GetSuperClass()->GetClassName() << endl;
    cout << endl;
}

int main(int argc, char** argv)
{
    MyTestObject* obj = GXAllocateFromType(MyTestObject);
    MyTestObject2* obj2 = GXAllocateFromType(MyTestObject2);
    MyTestObject3* obj3 = GXAllocateFromType(MyTestObject3);
    
    cout << endl;
    
    if(obj) obj->Initialize();
    if(obj2) obj2->Initialize();
    if(obj3) obj3->Initialize();
    
    DumpObject(obj);
    DumpObject(obj2);
    DumpObject(obj3);
    
    cout << GXTypeIDOfInstance(obj)->GetClassName() << " and " << GXTypeIDOfInstance(obj2)->GetClassName() << " are " << (GXCheckTypeInst(obj, obj2) ? "" : "not ") << "type compatible" << endl;
    cout << GXTypeIDOfInstance(obj)->GetClassName() << " and " << GXTypeIDOfInstance(obj3)->GetClassName() << " are " << (GXCheckTypeInst(obj, obj3) ? "" : "not ") << "type compatible" << endl;
    cout << GXTypeIDOfInstance(obj3)->GetClassName() << " and " << GXTypeIDOfInstance(obj)->GetClassName() << " are " << (GXCheckTypeInst(obj3, obj) ? "" : "not ") << "type compatible" << endl;
    
    cout << endl;
    
    MyTestObject* down = obj3;   //downcast
    MyTestObject3* up = GXDynamicCast(MyTestObject3, down);  //upcast
    if(up)
        cout << "Upcast was successful" << endl;
    
    cout << endl;
    
    if(GXDynamicCast(MyTestObject2, down))  //perform invalid cast
        cout << "Invalid cast from MyTestObject3 to MyTestObject2 didn't return NULL" << endl;
    else
        cout << "Invalid cast from MyTestObject3 to MyTestObject2 returned NULL" << endl;
    
    cout << endl;
    
    obj->Release();
    GXSafeReleaseNULL(obj2);
    GXSafeRelease(obj3);
    
    return 0;
}





GXImplementMetaClassAndDefaultStructors(MyTestObject, GXObject)

bool MyTestObject::Initialize()
{
    a = 0x41;
    return true;
}


GXImplementMetaClassAndDefaultStructors(MyTestObject2, GXObject)

bool MyTestObject2::Initialize()
{
    a = 0x42;
    return true;
}

GXImplementMetaClassAndDefaultStructors(MyTestObject3, MyTestObject)

bool MyTestObject3::Initialize()
{
    a = 0x43;
    return true;
}