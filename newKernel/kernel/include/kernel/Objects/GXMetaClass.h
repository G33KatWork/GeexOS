#ifndef _GX_OBJECT_SYSTEM_META_INFORMATION_H_
#define _GX_OBJECT_SYSTEM_META_INFORMATION_H_

#include <types.h>

class GXMetaClass;
class GXObject;

/**
 * This is the absolute base class for the object system.
 * It defines several helper macros for easy interfacing various methods
 * and defines the abstract interface for object allocation and reference
 * counting.
**/
class GXMetaClassBase
{
public:

/**
 * Allocates an instance of a given type
 * The caller is responsible for a check if the allocation was sucessful
**/
#define GXAllocateFromType(T)   ((T*) ((T::metaClass)->Allocate()))

/**
 * Returns the Type ID of a given type
 * The type ID is a pointer to a GXMetaClass
**/
#define GXTypeID(T)   (T::metaClass)

/**
 * Returns the Type ID of a given instantiated object
 * The type ID is a pointer to a GXMetaClass
**/
#define GXTypeIDOfInstance(instance)   ((instance)->GetMetaClass())

/**
 * tries to cast a given instance to a given type. returns NULL if not sucessful
**/
#define GXDynamicCast(type, instance)     ((type *) GXMetaClassBase::SafeMetaCast((instance), GXTypeID(type)))

/**
 * checks wether the types of two given instances are type compatible
 * i.e. is inst castable into typeinst
**/
#define GXCheckTypeInst(typeinst, inst)   GXMetaClassBase::CheckTypeInst(inst, typeinst)

/**
 * Releases a given instance if it is not NULL
**/
#define GXSafeRelease(instance)       do { if (instance) (instance)->Release(); } while (0)

/**
 * Releases a given instance if it is not NULL and sets the pointer to NULL
**/
#define GXSafeReleaseNULL(instance)   do { if (instance) (instance)->Release(); (instance) = NULL; } while (0)

protected:
    // We are an abstract base class...
    GXMetaClassBase();
    virtual ~GXMetaClassBase();

private:
    // Make copy constructors private to deactivate them
    // These declareations disable the 2 functions. They are not implemented
    void operator=(GXMetaClassBase &other);
    GXMetaClassBase(GXMetaClassBase &other);

public:
    /**
     * Returns the current retain count
    **/
    virtual int GetRetainCount() const = 0;

    /**
     * Retains an object
     * This methods has to be called everytime a pointer to a specific
     * instance is saved somewhere else. To decrement to reference
     * counter, use Release()
    **/
    virtual void Retain() const = 0;
    
    /**
     * Releases an object
     * This method frees the object if no references are held by somebody
     * else i.e. the reference count reaches 0
    **/
    virtual void Release() const = 0;

    /**
     * Returns the meta class ob this instance
     * The GXTypeIDOfInstance() macro can be used instead
    **/
    virtual const GXMetaClass* GetMetaClass() const = 0;

    /**
     * Checks wether two instances are equal
     * This functions has to be overridden by a subclass to consider
     * specific things which specify equality of two objects
    **/
    virtual bool IsEqual(const GXMetaClassBase* other) const;
    
    /**
     * Casts this object to another object type
     * If the cast is not sucessful, NULL is returned
    **/
    GXMetaClassBase* MetaCast(const GXMetaClass* toType) const;
    //GXMetaClassBase* MetaCast(const char* toMeta) const;
    
    /**
     * Safely casts a given object to a given type.
     * If the cast is not sucessful, NULL is returned
    **/
    static GXMetaClassBase* SafeMetaCast(const GXMetaClassBase* anObject, const GXMetaClass* toType);
    
    /**
     * Checks wether an instance of an object is type compatible (i.e. castable)
     * into another given type
    **/
    static bool CheckTypeInst(const GXMetaClassBase* anObject, const GXMetaClassBase* type);
    
    //static void Initialize(void);
};

/**
 * This class manages all runtime type information for all classes
 * in the framework. It is responsible for tracking inheritance trees, etc.
**/
class GXMetaClass : public GXMetaClassBase
{
private:
    /**
     * By making the new operator private for this class,
     * we can prevent dynamic creation of objects from this class
     * It only lives statically allocated in the class it manages the
     * metadata for
    **/
    static void* operator new(size_t size) throw();
    
    /**
     * A pointer to the class' superclass meta class
    **/
    const GXMetaClass* superClass;
    
    /**
     * The represented class' name
    **/
    const char* className;
    
    /**
     * The size of one instance of the class represented by this metaclass in bytes
    **/
    size_t instanceSize;
    
    /**
     * Private default constructor
    **/
    GXMetaClass();
    
public:
    /**
     * Find the metadata of a given class name
    **/
    //FIXME: to be implemented
    //static const GXMetaClass* FindMetaClassByName(const GXSymbol* symbol);
    
protected:
    /**
     * This class exists statically for every type defined
     * Because of this we don't need reference counting
     * This function does nothing
    **/
    virtual void Retain() const;
    
    /**
     * This class exists statically for every type defined
     * Because of this we don't need reference counting
     * This function does nothing
    **/
    virtual void Release() const;
    
    /**
     * Returns the metaclass of this class
    **/
    virtual const GXMetaClass * GetMetaClass() const;
    
    /**
     * Returns the retain count
     * Since this class is statically allocated, it always returns 0
    **/
    virtual int GetRetainCount() const;
    
    /**
     * Constructor, called as a base constructor from a specific type meta class
     * by a macro like GXImplementMetaClassWithInit()
    **/
    GXMetaClass(const char* className, const GXMetaClass* superClass, size_t instanceSize);
    
    /**
     * Destructor
    **/
    virtual ~GXMetaClass();
    
    /**
     * Since this class cannot be dynamically allocated, delete does noting
    **/
    void operator delete(void*, size_t);
    
public:
    /**
     * A pointer to the meta class which describes the meta class itself
    **/
    static const GXMetaClass* const metaClass;
    
    /**
     * Allocates an instance of a class identified by the given symbolname
    **/
    //FIXME: to be implemented
    //static GXObject* AllocateClassByName(const char* name);
    
    /**
     * Returns the super metaclass for this object
    **/
    const GXMetaClass* GetSuperClass() const;
    
    /**
     * Returns the name of the class represented by this metaclass
    **/
    const char* GetClassName() const;
    
    /**
     * Returns the size of an object of the represented class in bytes
    **/
    size_t GetInstanceSize() const;
    
    /**
     * Allocates memory for an instance of the represented class
    **/
    virtual GXObject* Allocate() const = 0;
    
    
    /**
     * Check wether the given instance of GXMetaClassBase is an
     * instance or child in the inheritance chain of this meta class
    **/
    GXMetaClassBase* CheckMetaCast(const GXMetaClassBase* check) const;
};


/**
 * This macro defines a meta class specific for the given class name
 * It also defines a protected constructur which takes a GXMetaData
 * pointer and virtual destructor
**/
#define GXDeclareCommonMetadata(className)                          \
    private:                                                        \
    static const GXMetaClass* const superClass;                     \
    public:                                                         \
        static const GXMetaClass* const metaClass;                  \
            static class MetaClass : public GXMetaClass {           \
            public:                                                 \
                MetaClass();                                        \
                virtual GXObject *Allocate() const;                 \
            } gMetaClass;                                           \
            friend class className ::MetaClass;                     \
            virtual const GXMetaClass* GetMetaClass() const;        \
        protected:                                                  \
        className (const GXMetaClass *);                            \
        virtual ~ className ()

/**
 * This macro defines the CommonMetadata as well as a public constructor
**/
#define GXDeclareDefaultMetadata(className)                         \
    GXDeclareCommonMetadata(className);                             \
    public:                                                         \
    className ();                                                   \
    protected:

/**
 * This macro defines the CommonMetadata as well as a private constructor
**/
#define GXDeclareAbstractMetadata(className)                        \
    GXDeclareCommonMetadata(className);                             \
    private:                                                        \
    className ();                                                   \
    protected:

/**
 * This macro implements the meta class specific for an object.
 * Used by other macros. Do not use directly!
**/
#define GXImplementMetaClassWithInit(className, superclassName, initCode)                   \
    /* Class global data */                                                                 \
    className ::MetaClass className ::gMetaClass;                                           \
    const GXMetaClass * const className ::metaClass = & className ::gMetaClass;             \
    const GXMetaClass * const className ::superClass = & superclassName ::gMetaClass;       \
    /* Class member functions */                                                            \
    className :: className(const GXMetaClass *meta) : superclassName (meta) { }             \
    className ::~ className() { }                                                           \
    const GXMetaClass * className ::GetMetaClass() const { return &gMetaClass; }            \
    /* The ::MetaClass constructor */                                                       \
    className ::MetaClass::MetaClass()                                                      \
        : GXMetaClass(#className, className::superClass, sizeof(className))                 \
        { initCode; }

/**
 * This macro implements the Allocate() function and a constructor for a default class
 * Used by other macros. Do not use directly!
**/
#define GXImplementDefaultStructors(className, superclassName)      \
    GXObject* className ::MetaClass::Allocate() const               \
    { return new className; }                                       \
    className :: className () : superclassName (&gMetaClass) { }

/**
 * This macro implements the Allocate() for an abstract class
 * Used by other macros. Do not use directly!
**/
#define GXImplementAbstractStructors(className, superclassName)     \
    OXObject* className ::MetaClass::Allocate() const { return 0; }


/**
 * This macro implements the metadata class and default constructors for a normal class
 * with initialization code for the meta class
**/
#define GXImplementMetaClassAndDefaultStructorsWithInit(className, superclassName, initCode)    \
    GXImplementMetaClassWithInit(className, superclassName, initCode)                           \
    GXImplementDefaultStructors(className, superclassName)

/**
 * This macro implements the metadata class and abstract constructors for an abstract class
 * with initialization code for the meta class
**/
#define GXImplementMetaClassAndAbstractStructorsWithInit(className, superclassName, initCode)   \
    GXImplementMetaClassWithInit(className, superclassName, initCode)                           \
    GXImplementAbstractStructors(className, superclassName)

/**
 * This macro implements the metadata class and default constructors for a normal class
**/
#define GXImplementMetaClassAndDefaultStructors(className, superclassName)                      \
    GXImplementMetaClassWithInit(className, superclassName, ;)                                  \
    GXImplementDefaultStructors(className, superclassName)

/**
 * This macro implements the metadata class and abstract constructors for an abstract class
**/
#define GXImplementMetaClassAndAbstractStructors(className, superclassName)                     \
    GXImplementMetaClassWithInit(className, superclassName, ;)                                  \
    GXImplementAbstractStructors(className, superclassName)

#endif
