#include <debug.h>
#include <print.h>
#include <arch.h>

static DBGPORT enabledOutputPorts = DBGPORT_RS232;
static DBGPORT_RS232_PORTS messageComPort = DBGPORT_RS232_COM1;
static DBGPORT_RS232_PORTS gdbComPort = DBGPORT_RS232_COM2;

static char vprintf_buffer[1024];

void debug_enableOutputPort(DBGPORT port) { enabledOutputPorts |= port; }
void debug_disableOutputPort(DBGPORT port) { enabledOutputPorts &= ~port; }

void debug_setMessageComPort(DBGPORT_RS232_PORTS port)
{
    messageComPort = port;
    arch_serial_setup(port);
}

void debug_setGdbComPort(DBGPORT_RS232_PORTS port)
{
    gdbComPort = port;
    arch_serial_setup(port);
}

void debug_init()
{
    debug_enableOutputPort(DBGPORT_RS232);
    debug_setMessageComPort(DBGPORT_RS232_COM1);
    debug_setGdbComPort(DBGPORT_RS232_COM2);
}

DBGPORT_RS232_PORTS debug_getGdbComPort() { return gdbComPort; }

size_t debug_printf(const char* format, ...)
{
    va_list params;
    size_t  retval;

    va_start(params, format);
    retval = vsnprintf(vprintf_buffer, sizeof(vprintf_buffer), format, params);
    va_end(params);

    char* ptr = vprintf_buffer;
    while(*ptr)
    {
        if((enabledOutputPorts & DBGPORT_SCREEN) != 0)
            arch_screen_putchar(*ptr);

        if((enabledOutputPorts & DBGPORT_RS232) != 0)
            arch_serial_putchar(messageComPort, *ptr);

        ptr++;
    }

    return retval;
}
