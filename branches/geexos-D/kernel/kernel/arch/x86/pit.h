#ifndef _PIC_H
#define _PIC_H

#include <stdint.h>

/**
 * Initialize the PIT
**/
void init_pit(uint32_t frequency);

/**
 * Get tick count
**/
uint32_t get_ticks(void);

#endif
