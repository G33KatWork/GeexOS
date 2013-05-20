#ifndef _RAND_H_
#define _RAND_H_

#include <stdint.h>

#define RAND_MAX 0xFFFFFFFF

void srand(uint32_t seed);
uint32_t rand(void);

#endif
