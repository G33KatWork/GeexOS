#include <arch/i386/serial.h>
#include <arch/i386/portio.h>

#define SERIAL_COM1  0x3f8

static int readyToTransmit(void);

unsigned short port;

void arch_i386_serial_setup()
{
	port = SERIAL_COM1;

	portio_outb(port + 1, 0x00);    // Disable all interrupts
	portio_outb(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	portio_outb(port + 0, 0x01);    // Set divisor to 1 (lo byte) 115200 baud
	portio_outb(port + 1, 0x00);    //                  (hi byte)
	portio_outb(port + 3, 0x03);    // 8 bits, no parity, one stop bit
	portio_outb(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	portio_outb(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
	portio_outb(port + 1, 0x0C);    // enable all interrupts.
}

void arch_i386_serial_print_char(char c)
{
	while(!readyToTransmit());
	portio_outb(port, c);
}

static int readyToTransmit()
{
	unsigned char status = portio_inb(port + 5);
	return (status & 0x20);
}
