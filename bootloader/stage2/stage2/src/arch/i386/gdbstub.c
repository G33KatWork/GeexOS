#include <arch/i386/gdbstub.h>
#include <arch.h>
#include <debug.h>
#include <lib.h>
#include <print.h>

#define BUFMAX 1000

bool gdbstub_inDebugger = false;

bool gdbstub_enabled = false;
static char inputBuffer[BUFMAX];
static char outputBuffer[BUFMAX];

static const char hexchars[]="0123456789abcdef";

#define NUMREGS         16
static uint32_t gdbRegisters[NUMREGS];

enum regnames {EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI,
               PC /* also known as eip */,
               PS /* also known as eflags */,
               CS, SS, DS, ES, FS, GS};

struct {
    size_t Size;
    uint16_t offsetTF;
    uint16_t offsetGDB;
} registerConversion[] = {
    {4, offsetof(trapframe, eax), 0},
    {4, offsetof(trapframe, ecx), 4},
    {4, offsetof(trapframe, edx), 8},
    {4, offsetof(trapframe, ebx), 12},
    {4, offsetof(trapframe, esp), 16},
    {4, offsetof(trapframe, ebp), 20},
    {4, offsetof(trapframe, esi), 24},
    {4, offsetof(trapframe, edi), 28},
    {4, offsetof(trapframe, eip), 32},
    {4, offsetof(trapframe, eflags), 36},
    {4, offsetof(trapframe, cs), 40},
    {4, offsetof(trapframe, ss), 44},
    {4, offsetof(trapframe, ds), 48},
    {4, offsetof(trapframe, es), 52},
    {4, offsetof(trapframe, fs), 56},
    {4, offsetof(trapframe, gs), 60}
};

//protocol request handlers
static void set_thread(char* request);
static void handle_query(char* request);

//packet parsing
static char* getpacket(void);
static void putpacket(char *buffer);

//conversion functions for protocol parsing
static int hex(char ch);
static char* mem2hex(char* mem, char* buf, int count);
static char* hex2mem(char* mem, char* buf, int count);
static int hexToInt(char **ptr, int *intValue);
static void import_registers(trapframe* ctx);
static void export_registers(trapframe* ctx);
static uint8_t gdbstub_i386_computeSignal(int exceptionVector);

void gdbstub_i386_enable()
{
    gdbstub_enabled = true;
}

void gdbstub_i386_handle_exception(trapframe* ctx)
{
    int addr, length;
    bool stepping = false;

    uint8_t unixSignal = gdbstub_i386_computeSignal(ctx->ex_no);
    char* ptr = outputBuffer;

    *ptr++ = 'T';
    *ptr++ = hexchars[unixSignal >> 4];
    *ptr++ = hexchars[unixSignal & 0xF];

    *ptr++ = hexchars[ESP];
    *ptr++ = ':';
    ptr = mem2hex((char*)&ctx->esp, ptr, 4);
    *ptr++ = ';';

    *ptr++ = hexchars[EBP];
    *ptr++ = ':';
    ptr = mem2hex((char*)&ctx->ebp, ptr, 4);
    *ptr++ = ';';

    *ptr++ = hexchars[PC];
    *ptr++ = ':';
    ptr = mem2hex((char*)&ctx->eip, ptr, 4);
    *ptr++ = ';';

    *ptr++ = '\0';

    putpacket(outputBuffer);

    while(1)
    {
        outputBuffer[0] = '\0';

        ptr = getpacket();
        debug_printf("Received: %s\n", ptr);
        switch(*ptr++)
        {
            case '?':
                outputBuffer[0] = 'S';
                outputBuffer[1] = hexchars[unixSignal >> 4];
                outputBuffer[2] = hexchars[unixSignal & 0x0F];
                outputBuffer[3] = '\0';
                break;

            case 'H':
                set_thread(ptr);
                break;

            case 'q':
                handle_query(ptr);
                break;

            //get all registers
            case 'g':
                import_registers(ctx);
                mem2hex((char *)gdbRegisters, outputBuffer, NUMREGS*4);
                break;

            //set all registers
            case 'G':
                hex2mem((char *)gdbRegisters, ptr, NUMREGS*4);
                export_registers(ctx);
                strcpy(outputBuffer, "OK");
                break;

            //set one register
            case 'P':
            {
                int regno;

                if (hexToInt (&ptr, &regno) && *ptr++ == '=')
                    if (regno >= 0 && regno < NUMREGS)
                    {
                        hex2mem((char*)&gdbRegisters[regno], ptr, 4);
                        export_registers(ctx);
                        strcpy(outputBuffer, "OK");
                        break;
                    }

                strcpy(outputBuffer, "E01");
                break;
            }

            /* mAA..AA,LLLL  Read LLLL bytes at address AA..AA */
            case 'm':
                /* TRY TO READ %x,%x.  IF SUCCEED, SET PTR = 0 */
                if(hexToInt(&ptr, &addr))
                    if(*(ptr++) == ',')
                        if(hexToInt(&ptr, &length))
                        {
                            ptr = NULL;
                            mem2hex((char *)addr, outputBuffer, length);
                            //on memory fault: strcpy(outputBuffer, "E03");
                        }

                //parse error?
                if (ptr)
                    strcpy(outputBuffer, "E01");
                break;

            /* MAA..AA,LLLL: Write LLLL bytes at address AA.AA return OK */
            case 'M':
                /* TRY TO READ %x,%x.  IF SUCCEED, SET PTR = 0 */
                if(hexToInt(&ptr, &addr))
                    if(*(ptr++) == ',')
                        if(hexToInt(&ptr, &length))
                            if(*(ptr++) == ':')
                            {
                                hex2mem((char *)addr, ptr, length);
                                //on memory fault: strcpy(outputBuffer, "E03");
                                strcpy(outputBuffer, "OK");
                                ptr = NULL;
                            }

                //parse error?
                if(ptr)
                    strcpy(outputBuffer, "E02");
                break;

            case 's':
                stepping = true;
                //explicit fall-through

            case 'c':
                if(hexToInt(&ptr, &addr))
                {
                    gdbRegisters[PC] = addr;
                    export_registers(ctx);
                }

                ctx->eflags &= 0xFFFFFEFF;

                if(stepping)
                    ctx->eflags |= 0x100;

                return;

            case 'k':
                //arch_panic("Killed by debugger");
                break;
        }

        putpacket(outputBuffer);
    }
}

static void set_thread(char* UNUSED(request))
{
    //return OK for now, we don't have any threads
    outputBuffer[0] = 'O';
    outputBuffer[1] = 'K';
    outputBuffer[2] = '\0';
}

static void handle_query(char* request)
{
    if(strncmp(request, "Supported", 9) == 0)
        snprintf(outputBuffer, BUFMAX, "PacketSize=%d", BUFMAX-1);
    else if(strncmp(request, "Symbol", 6) == 0)
        snprintf(outputBuffer, BUFMAX, "OK");
    else if(strncmp(request, "Offsets", 6) == 0)
        snprintf(outputBuffer, BUFMAX, "Text=0;Data=0;Bss=0;");
    else
        outputBuffer[0] = '\0';
}

static void import_registers(trapframe* ctx)
{
    for(int i = 0; i < NUMREGS; i++)
    {
        memcpy(
            (void*)(((Address)gdbRegisters) + registerConversion[i].offsetGDB),
            (void*)(((Address)ctx) + registerConversion[i].offsetTF),
            registerConversion[i].Size
        );
    }
}

static void export_registers(trapframe* ctx)
{
    for(int i = 0; i < NUMREGS; i++)
    {
        memcpy(
            (void*)(((Address)ctx) + registerConversion[i].offsetTF),
            (void*)(((Address)gdbRegisters) + registerConversion[i].offsetGDB),
            registerConversion[i].Size
        );
    }
}

static char* getpacket()
{
    char *buffer = &inputBuffer[0];
    unsigned char checksum;
    uint8_t xmitcsum;
    int count;
    char ch;

    DBGPORT_RS232_PORTS gdbPort = debug_getGdbComPort();

    while (1)
    {
        /* wait around for the start character, ignore all other characters */
        while ((ch = arch_serial_getchar(gdbPort)) != '$');

retry:
        checksum = 0;
        xmitcsum = -1;
        count = 0;

        /* now, read until a # or end of buffer is found */
        while (count < BUFMAX - 1)
        {
            ch = arch_serial_getchar(gdbPort);
            
            if (ch == '$')
                goto retry;
            
            if (ch == '#')
                break;
            
            checksum = checksum + ch;
            buffer[count] = ch;
            count = count + 1;
        }
        buffer[count] = 0;

        if (ch == '#')
        {
            ch = arch_serial_getchar(gdbPort);
            xmitcsum = hex (ch) << 4;

            ch = arch_serial_getchar(gdbPort);
            xmitcsum += hex (ch);

            if (checksum != xmitcsum)
                arch_serial_putchar (gdbPort, '-');   /* failed checksum */
            else
            {
                arch_serial_putchar (gdbPort, '+');   /* successful transfer */

            /* if a sequence char is present, reply the sequence ID */
            /*if (buffer[2] == ':')
            {
                arch_serial_putchar (gdbPort, buffer[0]);
                arch_serial_putchar (gdbPort, buffer[1]);

                return &buffer[3];
            }*/

                return &buffer[0];
            }
        }
    }
}

static void putpacket(char *buffer)
{
    uint8_t checksum;
    int count;
    char ch;

    DBGPORT_RS232_PORTS gdbPort = debug_getGdbComPort();
    debug_printf("GDBSTUB sending: %s\n", buffer);

    /*  $<packet info>#<checksum>.  */
    do
    {
        arch_serial_putchar(gdbPort, '$');
        checksum = 0;
        count = 0;

        while ((ch = buffer[count]))
        {
            arch_serial_putchar(gdbPort, ch);
            checksum += ch;
            count += 1;
        }

        arch_serial_putchar(gdbPort, '#');
        arch_serial_putchar(gdbPort, hexchars[(checksum >> 4) & 0x0F ]);
        arch_serial_putchar(gdbPort, hexchars[checksum & 0x0F]);
    }
    while (arch_serial_getchar(gdbPort) != '+');
}

static uint8_t gdbstub_i386_computeSignal(int exceptionVector)
{
    uint8_t sigval;
    switch (exceptionVector)
    {
        case 0:
            sigval = 8;
            break;          /* divide by zero */
        case 1:
            sigval = 5;
            break;            /* debug exception */
        case 3:
            sigval = 5;
            break;            /* breakpoint */
        case 4:
            sigval = 16;
            break;            /* into instruction (overflow) */
        case 5:
            sigval = 16;
            break;            /* bound instruction */
        case 6:
            sigval = 4;
            break;            /* Invalid opcode */
        case 7:
            sigval = 8;
            break;            /* coprocessor not available */
        case 8:
            sigval = 7;
            break;            /* double fault */
        case 9:
            sigval = 11;
            break;            /* coprocessor segment overrun */
        case 10:
            sigval = 11;
            break;            /* Invalid TSS */
        case 11:
            sigval = 11;
            break;            /* Segment not present */
        case 12:
            sigval = 11;
            break;            /* stack exception */
        case 13:
            sigval = 11;
            break;            /* general protection */
        case 14:
            sigval = 11;
            break;            /* page fault */
        case 16:
            sigval = 7;
            break;            /* coprocessor error */
        default:
            sigval = 7;       /* "software generated" */
    }

    return (sigval);
}

static int hex(char ch)
{
    if ((ch >= 'a') && (ch <= 'f'))
        return (ch - 'a' + 10);
    if ((ch >= '0') && (ch <= '9'))
        return (ch - '0');
    if ((ch >= 'A') && (ch <= 'F'))
        return (ch - 'A' + 10);
    return (-1);
}

static char* mem2hex(char* mem, char* buf, int count)
{
    int i;
    unsigned char ch;

    for (i = 0; i < count; i++)
    {
        ch = *(mem++);
        *buf++ = hexchars[ch >> 4];
        *buf++ = hexchars[ch % 16];
    }
    *buf = 0;
    return buf;
}

static char* hex2mem(char* mem, char* buf, int count)
{
    int i;
    unsigned char ch;

    for (i = 0; i < count; i++)
    {
        ch = hex (*buf++) << 4;
        ch = ch + hex (*buf++);
        *(mem++) = ch;
    }
    return mem;
}

static int hexToInt(char **ptr, int *intValue)
{
    int numChars = 0;
    int hexValue;

    *intValue = 0;

    while (**ptr)
    {
        hexValue = hex (**ptr);
        if (hexValue >= 0)
        {
            *intValue = (*intValue << 4) | hexValue;
            numChars++;
        }
        else
            break;

        (*ptr)++;
    }

    return (numChars);
}
