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
	};
}
