#ifndef _X86EMU_X86EMUTYPES_H
#define _X86EMU_X86EMUTYPES_H

typedef unsigned char 		u8;
typedef unsigned short 		u16;
typedef unsigned int 		u32;
typedef unsigned long long 	u64;

typedef char 				s8;
typedef short 				s16;
typedef int 				s32;
typedef long long 			s64;

#ifndef INKERNEL
typedef u64                 Address;
#endif

#endif
