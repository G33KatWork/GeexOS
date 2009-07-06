#ifndef _ARCH_TYPES_H
#define _ARCH_TYPES_H

/* unsigned */
typedef unsigned char uint8_t;		/**< Unsigned 8-bit. */
typedef unsigned short uint16_t;	/**< Unsigned 16-bit. */
typedef unsigned int uint32_t;		/**< Unsigned 32-bit. */
typedef unsigned long long uint64_t;	/**< Unsigned 64-bit. */

/* signed */
typedef signed char int8_t;		/**< Signed 8-bit. */
typedef signed short int16_t;		/**< Signed 16-bit. */
typedef signed int int32_t;		/**< Signed 32-bit. */
typedef signed long long int64_t;	/**< Signed 64-bit. */

typedef unsigned int Address;

#include <arch/registers.h>

#endif
