#ifndef _ARCH_IOAPIC_H
#define _ARCH_IOAPIC_H

#include <types.h>
#include <arch/internal/InterruptController.h>

// IOAPIC registers
#define		IOAPIC_ID            		    0x00
#define     IOAPIC_VERSION                  0x01
#define     IOAPIC_ARBITRATION_ID           0x02
#define     IOPAIC_REDIRECTION_TABLE_BEGIN  0x10

// IOAPIC memory mapped access registers
#define     IOAPIC_REGSEL                   0x00
#define     IOAPIC_IOWIN                    0x10

namespace Arch
{
    class IOAPIC : public InterruptController
    {
    public:
        IOAPIC(Address physicalBaseAddress) { baseAddress = physicalBaseAddress; }
        void MapIntoIOMemory();
        
        
        //Interrupt Controller interface
        virtual void Initialize() {}
        
        virtual void MaskVector(uint8_t vectorNumber)
        {
            (void)vectorNumber;
        }
        
        virtual void UnmaskVector(uint8_t vectorNumber)
        {
            (void)vectorNumber;
        }
        
        virtual bool EndOfInterrupt(uint8_t vectorNumber);
    
    private:
        Address GetVirtBaseAddress();
        
        inline uint32_t IOAPICRead32(uint8_t reg)
        {
            //Select register
            *(volatile uint32_t *)((char *)GetVirtBaseAddress() + IOAPIC_REGSEL) = reg;
            
            //read value from IO window
            return *(volatile uint32_t *)((char*)GetVirtBaseAddress() + IOAPIC_IOWIN);
        }
        
        inline void IOAPICWrite32(uint8_t reg, uint32_t data)
        {
            //Select register
            *(volatile uint32_t *)((char *)GetVirtBaseAddress() + IOAPIC_REGSEL) = reg;
            
            //write value to IO winodw
            *(volatile uint32_t *)((char *)GetVirtBaseAddress() + IOAPIC_IOWIN) = data;
        }
        
        inline uint64_t IOAPICRead64(uint8_t reg)
        {
            *(volatile uint32_t *)((char *)GetVirtBaseAddress() + IOAPIC_REGSEL) = reg + 1;
            uint64_t result = *(volatile uint32_t *)((char*)GetVirtBaseAddress() + IOAPIC_IOWIN);
            result <<= 32;
            *(volatile uint32_t *)((char *)GetVirtBaseAddress() + IOAPIC_REGSEL) = reg;
            result |= *(volatile uint32_t *)((char*)GetVirtBaseAddress() + IOAPIC_IOWIN);
            
            return result;
        }
        
        inline void IOAPICRead64(uint8_t reg, uint64_t data)
        {
            *(volatile uint32_t *)((char *)GetVirtBaseAddress() + IOAPIC_REGSEL) = reg;
            *(volatile uint32_t *)((char *)GetVirtBaseAddress() + IOAPIC_IOWIN) = (uint32_t)data;
            
            *(volatile uint32_t *)((char *)GetVirtBaseAddress() + IOAPIC_REGSEL) = reg + 1;
            *(volatile uint32_t *)((char *)GetVirtBaseAddress() + IOAPIC_IOWIN) = (uint32_t)(data >> 32);
        }
        
        
        Address baseAddress;
    };
}

#endif
