#ifndef _GX_OBJECT_SYSTEM_OBJECT_H_
#define _GX_OBJECT_SYSTEM_OBJECT_H_

#include <kernel/Objects/GXMetaClass.h>

/**
 * This is the base class of all objects inside the object
 * system. It implements basic allocation and deallocation
 * primitives as well as reference counting.
**/
class GXObject : public GXMetaClassBase
{
//Metadata class foo
//Move into macro later
GXDeclareAbstractMetadata(GXObject)
    
private:
    /**
     * The retain count of the object.
     * Marked as mutable for Retain() and Release().
     * These methods are const but need to operate on this value.
    **/
    mutable uint16_t retainCount;

protected:
    /**
     * Frees the object. Will be called when the retainCount reaches 0.
    **/
    static void operator delete(void* obj, size_t size);
    
    /**
     * Deallocates the object
    **/
    virtual void Free();
    
    /**
     * Initializes the object
    **/
    virtual bool Initialize();
    
public:
    /**
     * Allocates an object and returns an instance of it
    **/
    static void* operator new(size_t size);
    
    /**
     * Return the retain count of an object
    **/
    virtual int GetRetainCount() const;
    
    /**
     * Retains a reference to the object
    **/
    virtual void Retain() const;
    
    /**
     * Releases a reference from the object.
     * If the retainCount reaches 0 the object will be disposed
    **/
    virtual void Release() const;
};

#endif
