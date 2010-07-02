#ifndef _CXXABI_H
#define _CXXABI_H

#ifdef __cplusplus

/*-------------------------- RTTI stuff --------------------------*/

#include <typeinfo>

namespace __cxxabiv1
{
    //fundametal type info for char, float, int etc.
    class __fundamental_type_info : public std::type_info
    {
    public:
        virtual ~__fundamental_type_info();
    };
    
    //array type info
    class __array_type_info : public std::type_info
    {
    public:
        virtual ~__array_type_info();
    };
    
    //function type info
    class __function_type_info : public std::type_info
    {
    public:
        virtual ~__function_type_info();
    };
    
    //enum type info
    class __enum_type_info : public std::type_info
    {
    public:
        virtual ~__enum_type_info();
    };
    
    //class type info for classes with no base
    class __class_type_info : public std::type_info
    {
    public:
        virtual ~__class_type_info();
    };
    
    //class type info for classes with single, public, non-virtual base
    class __si_class_type_info : public __class_type_info
    {
    public:
        const __class_type_info *__base_type;
        virtual ~__si_class_type_info();
    };
    
    class __base_class_type_info
    {
    public:
        const __class_type_info *__base_type;
        long __offset_flags;

    	enum __offset_flags_masks
    	{
            __virtual_mask = 0x1,
            __public_mask = 0x2,
            __offset_shift = 8
        };
    };
    
    //class type info for all other classes
    class __vmi_class_type_info : public __class_type_info
    {
    public:
        virtual ~__vmi_class_type_info();
        
        unsigned int __flags;
        unsigned int __base_count;
        __base_class_type_info __base_info[1];

        enum __flags_masks
        {
            __non_diamond_repeat_mask = 0x1,
            __diamond_shaped_mask = 0x2
        };
    };
    
    //base type class for pointers and stuff
    class __pbase_type_info : public std::type_info
    {
    public:
        virtual ~__pbase_type_info();
        
        unsigned int __flags;
        const std::type_info* __pointee;

        //possible values for __flags
        enum __masks 
        {
            __const_mask = 0x1,
            __volatile_mask = 0x2,
            __restrict_mask = 0x4,
  	        __incomplete_mask = 0x8,
            __incomplete_class_mask = 0x10
        };
    };
    
    //generic pointer? don't know...
    class __pointer_type_info : public __pbase_type_info
    {
    public:
        virtual ~__pointer_type_info();
    };
    
    class __pointer_to_member_type_info : public __pbase_type_info
    {
    public:
        virtual ~__pointer_to_member_type_info();
        
        const __class_type_info *__context;
    };

} // namespace __cxxabiv1

/*-------------------------- Guard stuff --------------------------*/

namespace __cxxabiv1
{
    /* The ABI requires a 64-bit type.  */
    __extension__ typedef int __guard __attribute__((mode (__DI__)));

    extern "C" int __cxa_guard_acquire (__guard *);
    extern "C" void __cxa_guard_release (__guard *);
    extern "C" void __cxa_guard_abort (__guard *);
} // namespace __cxxabiv1

#endif // __cplusplus
#endif
