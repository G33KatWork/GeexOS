#include <arch/i386/serial.h>
#include <arch/i386/portio.h>

void arch_i386_serial_setup(uint16_t port)
{
	portio_outb(port + 1, 0x00);    // Disable all interrupts
	portio_outb(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	portio_outb(port + 0, 0x01);    // Set divisor to 1 (lo byte) 115200 baud
	portio_outb(port + 1, 0x00);    //                  (hi byte)
	portio_outb(port + 3, 0x03);    // 8 bits, no parity, one stop bit
	portio_outb(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	portio_outb(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
	portio_outb(port + 1, 0x0C);    // enable all interrupts.
}

void arch_i386_serial_put_char(uint16_t port, char c)
{
	while(!(portio_inb(port + 5) & 0x20));

	portio_outb(port, c);
}

char arch_i386_serial_get_char(uint16_t port)
{
	while(!(portio_inb(port + 5) & 0x01));

	return portio_inb(port);
}
