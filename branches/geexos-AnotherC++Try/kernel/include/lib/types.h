#ifndef _TYPES_H
#define _TYPES_H

#include <arch/types.h>

typedef __SIZE_TYPE__ size_t;		/**< Type to represent the size of an object. */

/** POSIX types. */
typedef int32_t ssize_t;		/**< Signed version of size_t. */

typedef int64_t time_t;			/**< Time. */
typedef int64_t clock_t;		/**< Clock. */
typedef uint64_t useconds_t;		/**< Microseconds. */
typedef int64_t suseconds_t;		/**< Signed microseconds. */

//typedef _Bool bool;			/**< Boolean type. */

//#define false		0		/**< False boolean value. */
//#define true		1		/**< True boolean value. */

#define NULL		0       	/**< NULL value for a pointer. */

/** Gets the offset of a member in a type. */
#define offsetof(type, member)	__builtin_offsetof(type, member)

#endif
