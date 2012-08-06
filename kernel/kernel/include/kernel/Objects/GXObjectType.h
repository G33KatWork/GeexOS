#ifndef _GEEXOS_OBJECT_TYPE_H_
#define _GEEXOS_OBJECT_TYPE_H_

#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <kernel/Memory/Slab/SlabAllocator.h>
#include <kernel/DataStructures/DoublyLinkedList.h>

namespace Objects
{
	class GXDirectoryObject;

	template<typename T>
	class GXObjectType;



	class GXBaseObject : public DataStructures::DoublyLinkedListLinkImpl<GXBaseObject>
	{
	protected:
		GXDirectoryObject* parent;
		const char* name;

		GXBaseObject(GXDirectoryObject* Parent, const char* Name)
		{		
			this->parent = Parent;
			this->name = Name;
		}

		virtual ~GXBaseObject()
		{
			//TODO: deregister from parent
		}

	public:
		virtual const char* GetName() { return this->name; }
		virtual GXDirectoryObject* GetParent() { return this->parent; }
		virtual const char* GetTypeName() { return "<untyped>"; }
	};



	template<typename T>
	class GXTypedObject : public GXBaseObject
	{
	friend class GXObjectType<T>;

	protected:
		GXObjectType<T>* type;

		GXTypedObject(GXObjectType<T>* Type, GXDirectoryObject* Parent, const char* Name);
		virtual ~GXTypedObject() { }

	public:
		void* operator new(size_t, void* buf) { return buf; }
        void operator delete(void* object) { ((T*)object)->type->DestroyObject((T*)object); }

        virtual const char* GetTypeName() { return this->type->GetTypeName(); }
	};

	template<typename T>
	GXTypedObject<T>::GXTypedObject(GXObjectType<T>* Type, GXDirectoryObject* Parent, const char* Name)
		: GXBaseObject(Parent, Name)
	{
		this->type = Type;
	}




	template<typename T>
	class GXObjectType
	{
	private:
		const char* typeName;
		Memory::Slab::SlabCache* slabCache;

	public:
		GXObjectType(const char* TypeName, size_t Alignment = 0)
		{
			this->typeName = TypeName;
			this->slabCache = Memory::VirtualMemoryManager::GetInstance()->SlabAllocator()->CreateCache(TypeName, sizeof(T), Alignment);
		}

		~GXObjectType()
		{
			Memory::VirtualMemoryManager::GetInstance()->SlabAllocator()->DestroyCache(this->slabCache);
		}

		const char* GetTypeName() { return this->typeName; }

		T* InstantiateObject(GXDirectoryObject* Parent, const char* Name)
		{
			void* buf = this->slabCache->AllocateObject();
			T* newObject = new(buf) T(this, Parent, Name);

			return newObject;
		}

		void DestroyObject(T* Object)
		{
			if(Object->type == this)
				this->slabCache->FreeObject(Object);
			else
				PANIC("Attempt to free object " << Object->GetName() <<
					" of type " << Object->type->typeName <<
					" via Type " << this->typeName
				);
		}
	};
}

#endif
