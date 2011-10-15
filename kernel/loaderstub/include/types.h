#ifndef _TYPES_H
#define _TYPES_H

typedef     __SIZE_TYPE__   size_t;      /**< Type to represent the size of an object. */

typedef     __UINT8_TYPE__  uint8_t;
typedef     __INT8_TYPE__   int8_t;

typedef     __UINT16_TYPE__ uint16_t;
typedef     __INT16_TYPE__  int16_t;

typedef     __UINT32_TYPE__ uint32_t;
typedef     __INT32_TYPE__  int32_t;

typedef     __UINT64_TYPE__ uint64_t;
typedef     __INT64_TYPE__  int64_t;

typedef     __UINTPTR_TYPE__ uintptr_t;
typedef     __INTPTR_TYPE__ intptr_t;

typedef     uintptr_t       Address;

typedef _Bool bool;			/**< Boolean type. */

#define FALSE		0		/**< False boolean value. */
#define TRUE		1		/**< True boolean value. */

#define NULL		((void*)0)      /**< NULL value for a pointer. */

/** Gets the offset of a member in a type. */
#define offsetof(type, member)	__builtin_offsetof(type, member)

#endif
