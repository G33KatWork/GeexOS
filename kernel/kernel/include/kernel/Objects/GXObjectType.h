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
		const char* name;

		GXBaseObject(const char* Name)
		{
			this->name = Name;
		}

		virtual ~GXBaseObject()
		{
			//TODO: deregister from parent
		}

	public:
		virtual const char* GetName() { return this->name; }
		virtual const char* GetTypeName() { return "<untyped>"; }
	};



	template<typename T>
	class GXTypedObject : public GXBaseObject
	{
	friend class GXObjectType<T>;

	protected:
		GXObjectType<T>* type;

		GXTypedObject(GXObjectType<T>* Type, const char* Name);
		virtual ~GXTypedObject() { }

	public:
		void* operator new(size_t, void* buf) { return buf; }
        void operator delete(void* object) { ((T*)object)->type->DestroyObject((T*)object); }

        virtual const char* GetTypeName() { return this->type->GetName(); }
	};

	template<typename T>
	GXTypedObject<T>::GXTypedObject(GXObjectType<T>* Type, const char* Name)
		: GXBaseObject(Name)
	{
		this->type = Type;
	}




	template<typename T>
	class GXObjectType : public GXBaseObject
	{
	private:
		Memory::Slab::SlabCache* slabCache;

	public:
		GXObjectType(const char* TypeName, size_t Alignment = 0)
			: GXBaseObject(TypeName)
		{
			this->slabCache = Memory::VirtualMemoryManager::GetInstance()->SlabAllocator()->CreateCache(TypeName, sizeof(T), Alignment);
		}

		~GXObjectType()
		{
			Memory::VirtualMemoryManager::GetInstance()->SlabAllocator()->DestroyCache(this->slabCache);
		}

		virtual const char* GetTypeName() { return "GXObjectType"; }

		T* InstantiateObject(const char* Name)
		{
			OBJ_SYS_DEBUG_MSG("Instantiating new object with name " << Name << " via type " << this->GetName());

			void* buf = this->slabCache->AllocateObject();
			T* newObject = new(buf) T(this, Name);

			return newObject;
		}

		void DestroyObject(T* Object)
		{
			OBJ_SYS_DEBUG_MSG("Destroying object " << Object->GetName() << " of type " << Object->GetTypeName() << " via type " << this->GetName());
			
			if(Object->type == this)
				this->slabCache->FreeObject(Object);
			else
				PANIC("Attempt to free object " << Object->GetName() <<
					" of type " << Object->GetTypeName() <<
					" via Type " << this->GetName()
				);
		}
	};
}

#endif
