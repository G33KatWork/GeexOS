#ifndef _GEEXOS_OBJECT_TYPE_H_
#define _GEEXOS_OBJECT_TYPE_H_

#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <kernel/Memory/Slab/SlabAllocator.h>

namespace Objects
{
	class GXDirectoryObject;

	template<typename T>
	class GXObjectType;

	template<typename T>
	class GXBaseObject
	{
	friend class GXObjectType<T>;

	protected:
		GXObjectType<T>* type;
		GXDirectoryObject* parent;
		const char* name;

		GXBaseObject(GXObjectType<T>* Type, GXDirectoryObject* Parent, const char* Name);
		virtual ~GXBaseObject() { }

	public:
		const char* GetName() { return this->name; }
		GXDirectoryObject* GetParent() { return this->parent; }

		void* operator new(size_t, void* buf) { return buf; }
        void operator delete(void* object) { ((T*)object)->type->DestroyObject((T*)object); }
	};

	template<typename T>
	GXBaseObject<T>::GXBaseObject(GXObjectType<T>* Type, GXDirectoryObject* Parent, const char* Name)
	{
		this->type = Type;
		this->parent = Parent;
		this->name = Name;
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
