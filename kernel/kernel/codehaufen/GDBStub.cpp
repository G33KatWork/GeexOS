#include <kernel/Debug/GDBStub.h>
#include <kernel/debug.h>
#include <lib/string.h>

using namespace Debug;
using namespace IO;

static const char hexchars[]="0123456789abcdef";

void GDBStub::TrapFromException(registers_t* regs)
{
    if(!IsEnabled()) return;
    
    GDBSTUB_DEBUG_MSG("Trapping from exception " << regs->int_no << " into GDB Stub");
    
    /* reply to host that an exception has occurred */
    int sigval = computeSignal(regs->int_no);
    outBuffer[0] = 'S';
    outBuffer[1] =  hexchars[sigval >> 4];
    outBuffer[2] =  hexchars[sigval % 16];
    outBuffer[3] = 0;

    putpacket(outBuffer);
    
    GDBSTUB_DEBUG_MSG("Notified GDB that an exception occoured");
    
    while(true)
    {
        getpacket(inBuffer);
        GDBSTUB_DEBUG_MSG("Received following command from gdb: " << inBuffer);
        
        memset(outBuffer, 0, BUFMAX);
        
        if(inBuffer[0] == '?')
        {
            outBuffer[0] = 'S';
            outBuffer[1] =  hexchars[sigval >> 4];
            outBuffer[2] =  hexchars[sigval % 16];
            outBuffer[3] = 0;
        }
        else if(inBuffer[0] == 'q') //general query packages
        {
            if(inBuffer[1] == 'C')  //return current thread id (which? seems to be the general id...)
            {
                memcpy(outBuffer, "QC", 2);
                outBuffer[2] = hexchars[curThreadIDGeneral >> 4];
                outBuffer[2] =  hexchars[sigval % 16];
            }
            else if(!strcmp("qSupported", inBuffer))
                memcpy(outBuffer, "PacketSize=399", 28);
        }
        else if(inBuffer[0] == 'H') //set Thread ID...
        {
            char* id = &inBuffer[2];
            if(inBuffer[1] == 'g')  //... for general operations
            {
                int newId;
                hexToInt(&id, &newId);
                curThreadIDGeneral = newId;
                GDBSTUB_DEBUG_MSG("Set current general thread id to " << dec << newId);
            }
            else if(inBuffer[1] == 'c') //... for stepping operations
            {
                int newId;
                hexToInt(&id, &newId);
                curThreadIDStepping = newId;
                GDBSTUB_DEBUG_MSG("Set current stepping thread id to " << dec << newId);
            }
            
            memcpy(outBuffer, "OK", 2);
        }
        
        
        putpacket(outBuffer);
    }
}

void GDBStub::getpacket(char* buffer)
{
    int count;
    unsigned char checksum;
    unsigned char recChecksum;
    char ch;
    
    do
    {
        //wait for start character
        while ((ch = (serial->ReadChar() & 0x7f)) != '$');
    
        count = 0;
        checksum = 0;
        recChecksum = 0xFF;
    
        //read whole packet into buffer
        while(count < BUFMAX)
        {
            ch = serial->ReadChar() & 0x7f;
            if (ch == '#') break;
            checksum = checksum + ch;
            buffer[count] = ch;
            count = count + 1;
        }
        buffer[count] = 0;
        
        if (ch == '#')
        {
            recChecksum = hexchar2int(serial->ReadChar() & 0x7f) << 4;
            recChecksum += hexchar2int(serial->ReadChar() & 0x7f);

            if (checksum != recChecksum)
            {
                serial->PrintChar('-');  /* failed checksum */
                GDBSTUB_DEBUG_MSG("Checksum for packet failed: " << buffer << " My Chk: " << hex << checksum << " Sent Chk: " << recChecksum);
            }
            else
            {
    	        serial->PrintChar('+');  /* successful transfer */
                GDBSTUB_DEBUG_MSG("Successfully received command: " << buffer);
    	        
    	        /* if a sequence char is present, reply the sequence ID */
    	        if (buffer[2] == ':')
    	        {
    	            serial->PrintChar( buffer[0] );
    	            serial->PrintChar( buffer[1] );
    	            /* remove sequence chars from buffer */
    	            count = strlen(buffer);
    	            for (int i = 3; i <= count; i++) buffer[i-3] = buffer[i];
    	        }
            }
        }
        
    } while(checksum != recChecksum);
}


void GDBStub::putpacket(char* buffer)
{
    unsigned char checksum;
    int  count;
    char ch;

    GDBSTUB_DEBUG_MSG("Sending packet with contents: " << buffer);

    /*  $<packet info>#<checksum>. */
    do
    {
        serial->PrintChar('$');
        checksum = 0;
        count    = 0;
        
        while (buffer[count])
    	{
            ch = buffer[count];
    	    serial->PrintChar(ch);
    	    checksum += ch;
    	    count += 1;
    	}

        serial->PrintChar('#');
        serial->PrintChar(hexchars[checksum >> 4]);
        serial->PrintChar(hexchars[checksum % 16]);

    }
    while((serial->ReadChar() & 0x7f) != '+');
}

int GDBStub::hexchar2int(char ch)
{
    if ((ch >= 'a') && (ch <= 'f')) return (ch-'a'+10);
    if ((ch >= '0') && (ch <= '9')) return (ch-'0');
    if ((ch >= 'A') && (ch <= 'F')) return (ch-'A'+10);
    return (-1);
}

/* this function takes the 386 exception vector and attempts to
   translate this number into a unix compatible signal value */
char GDBStub::computeSignal(char exceptionVector)
{
    int sigval;
    switch (exceptionVector)
    {
        case 0 : sigval = 8; break;     /* divide by zero */
        case 1 : sigval = 5; break;     /* debug exception */
        case 3 : sigval = 5; break;     /* breakpoint */
        case 4 : sigval = 16; break;    /* into instruction (overflow) */
        case 5 : sigval = 16; break;    /* bound instruction */
        case 6 : sigval = 4; break;     /* Invalid opcode */
        case 7 : sigval = 8; break;     /* coprocessor not available */
        case 8 : sigval = 7; break;     /* double fault */
        case 9 : sigval = 11; break;    /* coprocessor segment overrun */
        case 10 : sigval = 11; break;   /* Invalid TSS */
        case 11 : sigval = 11; break;   /* Segment not present */
        case 12 : sigval = 11; break;   /* stack exception */
        case 13 : sigval = 11; break;   /* general protection */
        case 14 : sigval = 11; break;   /* page fault */
        case 16 : sigval = 7; break;    /* coprocessor error */
        default : sigval = 7;           /* "software generated"*/
    }
    return sigval;
}

int GDBStub::hexToInt(char **ptr, int *intValue)
{
    bool negative = false;
    int numChars = 0;
    int hexValue;

    *intValue = 0;
    
    //check for negative number
    if(**ptr == '-')
    {
        negative = true;
        (*ptr)++;
    }

    while (**ptr)
    {
        hexValue = hexchar2int(**ptr);
        if (hexValue >=0)
        {
            *intValue = (*intValue <<4) | hexValue;
            numChars ++;
        }
        else
            break;

        (*ptr)++;
    }
    
    if(negative)
        *intValue = -*intValue;

    return numChars;
}
