#include <arch/internal/PIT.h>
#include <types.h>
#include <arch/internal/ports.h>
#include <arch/HAL.h>
#include <arch/x86InterruptDispatcher.h>

//-----------------------------------------------
//	Operational Command Bit masks
//-----------------------------------------------

#define		I86_PIT_OCW_MASK_BINCOUNT		1		//00000001
#define		I86_PIT_OCW_MASK_MODE			0xE		//00001110
#define		I86_PIT_OCW_MASK_RL				0x30	//00110000
#define		I86_PIT_OCW_MASK_COUNTER		0xC0	//11000000

//-----------------------------------------------
//	Operational Command control bits
//-----------------------------------------------

// Use when setting binary count mode
#define		I86_PIT_OCW_BINCOUNT_BINARY		0		//0
#define		I86_PIT_OCW_BINCOUNT_BCD		1		//1

// Use when setting counter mode
#define		I86_PIT_OCW_MODE_TERMINALCOUNT	0		//0000
#define		I86_PIT_OCW_MODE_ONESHOT		0x2		//0010
#define		I86_PIT_OCW_MODE_RATEGEN		0x4		//0100
#define		I86_PIT_OCW_MODE_SQUAREWAVEGEN	0x6		//0110
#define		I86_PIT_OCW_MODE_SOFTWARETRIG	0x8		//1000
#define		I86_PIT_OCW_MODE_HARDWARETRIG	0xA		//1010

// Use when setting data transfer
#define		I86_PIT_OCW_RL_LATCH			0			//000000
#define		I86_PIT_OCW_RL_LSBONLY			0x10		//010000
#define		I86_PIT_OCW_RL_MSBONLY			0x20		//100000
#define		I86_PIT_OCW_RL_DATA				0x30		//110000

// Use when setting the counter we are working with
#define		I86_PIT_OCW_COUNTER_0			0		//00000000
#define		I86_PIT_OCW_COUNTER_1			0x40	//01000000
#define		I86_PIT_OCW_COUNTER_2			0x80	//10000000

//-----------------------------------------------
//	Controller Registers
//-----------------------------------------------

#define		I86_PIT_REG_COUNTER0		0x40
#define		I86_PIT_REG_COUNTER1		0x41
#define		I86_PIT_REG_COUNTER2		0x42
#define		I86_PIT_REG_COMMAND			0x43

using namespace Arch;


void PIT::SetTickLengthUs(unsigned long us)
{
    curTickLen = us;
    uint32_t reloadVal = 1193180 * (us / 1000.0 / 1000.0);
	
	//Operational Command Word
	uint8_t ocw=0;
	ocw = (ocw & ~I86_PIT_OCW_MASK_MODE) | I86_PIT_OCW_MODE_SQUAREWAVEGEN;  //swithc mode to squarewave generation
	ocw = (ocw & ~I86_PIT_OCW_MASK_RL) | I86_PIT_OCW_RL_DATA;               //access low and high byte
	ocw = (ocw & ~I86_PIT_OCW_MASK_COUNTER) | I86_PIT_OCW_COUNTER_0;        //select channel 0
	outb (I86_PIT_REG_COMMAND, ocw);
	
	//Set frequency
	//essentially we just set the 16 bit reload counter
	outb (I86_PIT_REG_COUNTER0, reloadVal & 0xff);
	outb (I86_PIT_REG_COUNTER0, (reloadVal >> 8) & 0xff);
}

void PIT::WaitForTick()
{
    uint16_t curCount, prevCount;
    curCount = GetCurrentCount();
    do {
        prevCount = curCount;
        curCount = GetCurrentCount();
        
        // check for errornous results on Intel Neptune/Mercury/Aries Chipset 8237IB
        // if the read value differs too much from the previous value, we just read it again
        if(prevCount >= curCount)
            curCount = GetCurrentCount();
            
    } while(prevCount >= curCount);
}

uint16_t PIT::GetCurrentCount()
{
    uint16_t count;
    //latch the current count value of channel 0
    outb(I86_PIT_REG_COMMAND, I86_PIT_OCW_COUNTER_0 | I86_PIT_OCW_RL_LATCH);
    //get the count value
    count = inb(I86_PIT_REG_COUNTER0);
    count |= inb(I86_PIT_REG_COUNTER0) << 8;
    return count;
}
