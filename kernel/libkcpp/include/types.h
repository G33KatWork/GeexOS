#ifndef _TYPES_H
#define _TYPES_H

typedef __SIZE_TYPE__ size_t;		/**< Type to represent the size of an object. */

//typedef _Bool bool;			/**< Boolean type. */

//#define false		0		/**< False boolean value. */
//#define true		1		/**< True boolean value. */

#define NULL		0       	/**< NULL value for a pointer. */

/** Gets the offset of a member in a type. */
#define offsetof(type, member)	__builtin_offsetof(type, member)

#endif
