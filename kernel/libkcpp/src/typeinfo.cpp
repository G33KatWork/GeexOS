#include <cxxabi.h>

std::type_info::~type_info() {}

namespace __cxxabiv1
{
using namespace std;

//the definitions of the destructors let the gcc emit vtables for the types
//which are needed for linkage (at least it seems so - without i get linker errors)
__fundamental_type_info::~__fundamental_type_info() {}
__array_type_info::~__array_type_info() {}
__function_type_info::~__function_type_info() {}
__enum_type_info::~__enum_type_info() {}
__class_type_info::~__class_type_info() {}
__si_class_type_info::~__si_class_type_info() {}
__vmi_class_type_info::~__vmi_class_type_info() {}
__pbase_type_info::~__pbase_type_info() {}
__pointer_type_info::~__pointer_type_info() {}
__pointer_to_member_type_info::~__pointer_to_member_type_info() {}

}
