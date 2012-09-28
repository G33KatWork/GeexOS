#include <kernel/Objects/GXDirectoryObject.h>

namespace Objects
{
	class ObjectManager
	{
	private:
		GXDirectoryObject* root;

	public:
		ObjectManager();
		~ObjectManager();

		GXBaseObject* InstantiateObjectByType(const char* Typename, const char* Name);
		GXBaseObject* FindObjectByName(const char* Name);

		void DumpTree(Debug::BaseOutputDevice* c);

	private:
		void PrintLevel(Debug::BaseOutputDevice* c, GXDirectoryObject* dir, int level);
	};
}
