#ifndef _LDR_GEEXOS_H_
#define _LDR_GEEXOS_H_

#include <types.h>

#define	GEEXOS_KERNEL_STACK_ADDRESS	0xE0000000
#define	GEEXOS_KERNEL_STACK_SIZE	0x4000

void loader_loadGeexOS(void);

#endif
