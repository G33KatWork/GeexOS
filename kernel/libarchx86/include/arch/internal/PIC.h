#ifndef _ARCHX86_PIC_H
#define _ARCHX86_PIC_H

#include <types.h>
#include <arch/internal/InterruptController.h>

//-----------------------------------------------
//	Command words are used to control the devices
//-----------------------------------------------

// Command Word 2 bit masks. Use when sending commands
#define		I86_PIC_OCW2_MASK_L1		1		//00000001
#define		I86_PIC_OCW2_MASK_L2		2		//00000010
#define		I86_PIC_OCW2_MASK_L3		4		//00000100
#define		I86_PIC_OCW2_MASK_EOI		0x20	//00100000
#define		I86_PIC_OCW2_MASK_SL		0x40	//01000000
#define		I86_PIC_OCW2_MASK_ROTATE	0x80	//10000000

// Command Word 3 bit masks. Use when sending commands
#define		I86_PIC_OCW3_MASK_RIS		1		//00000001
#define		I86_PIC_OCW3_MASK_RIR		2		//00000010
#define		I86_PIC_OCW3_MASK_MODE		4		//00000100
#define		I86_PIC_OCW3_MASK_SMM		0x20	//00100000
#define		I86_PIC_OCW3_MASK_ESMM		0x40	//01000000
#define		I86_PIC_OCW3_MASK_D7		0x80	//10000000

namespace Arch
{
    class PIC : public InterruptController
    {
    private:
        unsigned char master_mask;
        unsigned char slave_mask;
        
    public:
        PIC() {}
        
        virtual void Initialize();
        
        virtual void MaskVector(uint8_t vectorNumber);
        virtual void UnmaskVector(uint8_t vectorNumber);
        
        virtual bool EndOfInterrupt(uint8_t vectorNumber);
    };
}

#endif
