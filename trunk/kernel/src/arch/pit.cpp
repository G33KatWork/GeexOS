#include <arch/pit.h>
#include <lib/types.h>
#include <arch/interrupts.h>
#include <arch/ports.h>

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

PIT* PIT::instance = NULL;

PIT* PIT::GetInstance()
{
    if(instance == NULL)
        instance = new PIT();
    
    return instance;
}

PIT::PIT()
{
    ticks = 0;
}

void PIT::Initialize(uint32_t frequency)
{
    InterruptDispatcher::GetInstance()->RegisterHandler(IRQ_TIMER, this);
	uint32_t divider = 1193180 / frequency;
	
	//Operational Command Word
	uint8_t ocw=0;
	ocw = (ocw & ~I86_PIT_OCW_MASK_MODE) | I86_PIT_OCW_MODE_SQUAREWAVEGEN;
	ocw = (ocw & ~I86_PIT_OCW_MASK_RL) | I86_PIT_OCW_RL_DATA;
	ocw = (ocw & ~I86_PIT_OCW_MASK_COUNTER) | I86_PIT_OCW_COUNTER_0;
	outb (I86_PIT_REG_COMMAND, ocw);
	
	//Set frequency
	outb (I86_PIT_REG_COUNTER0, divider & 0xff);
	outb (I86_PIT_REG_COUNTER0, (divider >> 8) & 0xff);
}

long PIT::GetTickCount()
{
    return ticks;
}

void PIT::Execute(registers_t regs)
{
    ticks++;
}
