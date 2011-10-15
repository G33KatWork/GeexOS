#ifndef _LIMITS_H
#define _LIMITS_H

#define UCHAR_MAX       0xffU                   /* max value for an unsigned char */
#define SCHAR_MAX       __SCHAR_MAX__           /* max value for a char */
#define SCHAR_MIN       (-__SCHAR_MAX__-1)      /* min value for a char */

#define USHRT_MAX       0xffffU                 /* max value for an unsigned short */
#define SHRT_MAX        __SHRT_MAX__            /* max value for a short */
#define SHRT_MIN        (-__SHRT_MAX__-1)       /* min value for a short */

#define UINT_MAX        0xffffffffU             /* max value for an unsigned int */
#define INT_MAX         __INT_MAX__             /* max value for an int */
#define INT_MIN         (-__INT_MAX__-1)        /* min value for an int */

# define ULONG_MAX      0xffffffffUL            /* max value for an unsigned long */
# define LONG_MAX       __LONG_MAX__            /* max value for a long */
# define LONG_MIN       (-__LONG_MAX__-1)       /* min value for a long */

#define SIZE_MAX        __SIZE_MAX__

#endif
