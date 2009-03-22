#include "serial.h"
#include "../arch/x86/utils.h"
#include <stdarg.h>
#include <string.h>

#define SET_INTERRUPT_PORT  0x1
#define SET_INTERRUPTS_OFF	0x0

#define	SET_BAUDRATE_PORT	0x3
#define	SET_BAUDRATE		0x80

#define BAUDRATE_HIGH_PORT	0x0
#define BAUDRATE_LOW_PORT	0x1

#define SET_8N1_PORT		0x3
#define SET_8N1				0x3

#define PORTSTATE_PORT		0x5
#define PORTSTATE_TRANSMIT_EMPTY	0x20

static int is_transmit_empty(uint16_t portnum) {
   return inportb(portnum + PORTSTATE_PORT) & PORTSTATE_TRANSMIT_EMPTY;
}


void init_serial(uint16_t portnum, uint32_t baudrate)
{
	outportb(portnum + SET_INTERRUPT_PORT, SET_INTERRUPTS_OFF);
	outportb(portnum + SET_BAUDRATE_PORT, SET_BAUDRATE);
	
	uint16_t divider = 115200 / baudrate;
	outportb(portnum + BAUDRATE_HIGH_PORT, (uint8_t)((divider & 0xFF00) >> 8));
	outportb(portnum + BAUDRATE_LOW_PORT, (uint8_t)(divider & 0x00FF) );
	
	outportb(portnum + SET_8N1_PORT, SET_8N1);
	
		/*outportb(portnum + 1, 0x00);    // Disable all interrupts
		   outportb(portnum + 3, 0x80);    // Enable DLAB (set baud rate divisor)
		   outportb(portnum + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
		   outportb(portnum + 1, 0x00);    //                  (hi byte)
		   outportb(portnum + 3, 0x03);    // 8 bits, no parity, one stop bit
		   outportb(portnum + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
		   outportb(portnum + 4, 0x0B);    // IRQs enabled, RTS/DSR set*/
}

int printf_serial(uint16_t portnum, const char* str, ...)
{
	if(!str)
		return 0;

	va_list		args;
	va_start (args, str);
	size_t i;
	for (i=0; i<strlen(str);i++) {

		switch (str[i]) {

			case '%':

				switch (str[i+1]) {

					/*** characters ***/
					case 'c': {
						char c = (char) va_arg (args, int);
						putc_serial (portnum, c);
						i++;		// go to next character
						break;
					}

					/*** address of ***/
					case 's': {
						int* c = (int*) va_arg (args, int);
						char str2[64];
						strcpy (str2,(const char*)c);
						puts_serial (portnum, str2);
						i++;		// go to next character
						break;
					}

					/*** integers ***/
					case 'd':
					case 'i': {
						int c = va_arg (args, int);
						char str2[32]={0};
						itoa_s (c, 10, str2);
						puts_serial (portnum, str2);
						i++;		// go to next character
						break;
					}

					case 'u': {
						int c = va_arg (args, unsigned);
						char str2[32]={0};
						itoa (c, 10, str2);
						puts_serial (portnum, str2);
						i++;		// go to next character
						break;
					}

					/*** display in hex ***/
					case 'X':
					case 'x': {
						int c = va_arg (args, int);
						char str2[32]={0};
						itoa (c,16,str2);
						puts_serial (portnum, str2);
						i++;		// go to next character
						break;
					}

					default:
						va_end (args);
						return 1;
				}

				break;

			default:
				putc_serial (portnum, str[i]);
				break;
		}

	}

	va_end (args);
	return i;
}

void putc_serial(uint16_t portnum, char c)
{
	while(is_transmit_empty(portnum) == 0);
	outportb(portnum, (uint8_t)c);
}

void puts_serial (uint16_t portnum, const char* str)
{
	if (!str)
		return;

    for (unsigned int i=0; i<strlen(str); i++)
        putc_serial (portnum, str[i]);
}
