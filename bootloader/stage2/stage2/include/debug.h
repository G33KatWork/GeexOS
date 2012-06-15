#ifndef _GXLDR_DEBUG_H_
#define _GXLDR_DEBUG_H_

#include <types.h>

typedef enum { 
	DBGPORT_SCREEN		=	0x01,
	DBGPORT_RS232		=	0x02
} DBGPORT;

typedef enum {
	DBGPORT_RS232_COM1	=	0x01,
	DBGPORT_RS232_COM2	=	0x02,
	DBGPORT_RS232_COM3	=	0x04,
	DBGPORT_RS232_COM4	=	0x08
} DBGPORT_RS232_PORTS;

void debug_enableOutputPort(DBGPORT port);
void debug_disableOutputPort(DBGPORT port);
void debug_setMessageComPort(DBGPORT_RS232_PORTS port);
void debug_setGdbComPort(DBGPORT_RS232_PORTS port);
DBGPORT_RS232_PORTS debug_getGdbComPort(void);

void debug_init(void);

size_t debug_printf(const char*, ...);

#endif
