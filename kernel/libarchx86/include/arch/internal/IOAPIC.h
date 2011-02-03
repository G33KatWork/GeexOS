#ifndef _ARCH_IOAPIC_H
#define _ARCH_IOAPIC_H

#include <types.h>
#include <arch/internal/InterruptController.h>
#include <arch/internal/ACPI/MADT.h>

// IOAPIC registers
#define		IOAPIC_ID            		    0x00
#define     IOAPIC_VERSION                  0x01
#define     IOAPIC_ARBITRATION_ID           0x02
#define     IOPAIC_REDIRECTION_TABLE_BEGIN  0x10

// IOAPIC memory mapped access registers
#define     IOAPIC_REGSEL                   0x00
#define     IOAPIC_IOWIN                    0x10

// Fields for the ID register
#define     IOAPIC_ID_SHIFT                 24
#define     IOAPIC_ID_MASK                  0x0F

// Fields for the version register
#define     IOAPIC_VERSION_SHIFT				0
#define     IOAPIC_VERSION_MASK				    0xff
#define     IOAPIC_MAX_REDIRECTION_ENTRY_SHIFT	16
#define     IOAPIC_MAX_REDIRECTION_ENTRY_MASK	0xff

// Fields of each redirection table entry
#define     IOAPIC_DESTINATION_FIELD_SHIFT		56
#define     IOAPIC_DESTINATION_FIELD_MASK		0x0f
#define     IOAPIC_INTERRUPT_MASK_SHIFT		    16
#define     IOAPIC_INTERRUPT_MASKED			    1
#define     IOAPIC_INTERRUPT_UNMASKED			0
#define     IOAPIC_TRIGGER_MODE_SHIFT			15
#define     IOAPIC_TRIGGER_MODE_EDGE			0
#define     IOAPIC_TRIGGER_MODE_LEVEL			1
#define     IOAPIC_REMOTE_IRR_SHIFT		    	14
#define     IOAPIC_PIN_POLARITY_SHIFT			13
#define     IOAPIC_PIN_POLARITY_HIGH_ACTIVE	    0
#define     IOAPIC_PIN_POLARITY_LOW_ACTIVE		1
#define     IOAPIC_DELIVERY_STATUS_SHIFT		12
#define     IOAPIC_DELIVERY_STATUS_IDLE		    0
#define     IOAPIC_DELIVERY_STATUS_PENDING		1
#define     IOAPIC_DESTINATION_MODE_SHIFT		11
#define     IOAPIC_DESTINATION_MODE_PHYSICAL	0
#define     IOAPIC_DESTINATION_MODE_LOGICAL	    1
#define     IOAPIC_DELIVERY_MODE_SHIFT			8
#define     IOAPIC_DELIVERY_MODE_MASK			0x07
#define     IOAPIC_DELIVERY_MODE_FIXED			0
#define     IOAPIC_DELIVERY_MODE_LOWEST_PRIO	1
#define     IOAPIC_DELIVERY_MODE_SMI			2
#define     IOAPIC_DELIVERY_MODE_NMI			4
#define     IOAPIC_DELIVERY_MODE_INIT			5
#define     IOAPIC_DELIVERY_MODE_EXT_INT		7
#define     IOAPIC_INTERRUPT_VECTOR_SHIFT		0
#define     IOAPIC_INTERRUPT_VECTOR_MASK		0xff

namespace Arch
{
    class IOAPIC : public InterruptController
    {
    public:
        IOAPIC(Address physicalBaseAddress, uint32_t interruptBase)
        {
            baseAddress = physicalBaseAddress;
            globalSystemInterruptBase = interruptBase;
        }
        void MapIntoIOMemory();
        
        uint32_t GetID() { return (IOAPICRead32(IOAPIC_ID) >> IOAPIC_ID_SHIFT) & IOAPIC_ID_MASK; }
        void SetID(uint32_t id)
        {
            uint32_t newId = IOAPICRead32(IOAPIC_ID) & ~(IOAPIC_ID_MASK << IOAPIC_ID_SHIFT);
            newId |= id << IOAPIC_ID_SHIFT;
            IOAPICWrite32(IOAPIC_ID, newId);
        }
        uint32_t GetVersion() { return (IOAPICRead32(IOAPIC_VERSION) >> IOAPIC_VERSION_SHIFT) & IOAPIC_VERSION_MASK; }
        
        uint32_t GetGlobalSystemInterruptBase() { return globalSystemInterruptBase; }
        
        /**
         * This method handles an interrupt override as described in an
         * ACPI InterruptSourceOverrideStructure.
         * It is assumed that the IOAPIC has the standard legacy x86 mapping
         * of the interrupt vectors. With this method a standard vector can
         * be "rewired" to a different.
         *
         * @param standardVector The original legacy x86 vector
         * @param overriddenVector The new vector for the device
        **/
        void HandleInterruptOverride(uint8_t standardVector, uint8_t overriddenVector, ACPI::InterruptSourceOverrideStructureFlags flags);
        
        //Interrupt Controller interface
        virtual void Initialize();
        
        virtual void MaskVector(uint8_t vectorNumber);
        virtual void UnmaskVector(uint8_t vectorNumber);
        
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
        
        inline void IOAPICWrite64(uint8_t reg, uint64_t data)
        {
            *(volatile uint32_t *)((char *)GetVirtBaseAddress() + IOAPIC_REGSEL) = reg;
            *(volatile uint32_t *)((char *)GetVirtBaseAddress() + IOAPIC_IOWIN) = (uint32_t)data;
            
            *(volatile uint32_t *)((char *)GetVirtBaseAddress() + IOAPIC_REGSEL) = reg + 1;
            *(volatile uint32_t *)((char *)GetVirtBaseAddress() + IOAPIC_IOWIN) = (uint32_t)(data >> 32);
        }
        
        
        Address baseAddress;
        uint32_t globalSystemInterruptBase;
        uint8_t irqToAPICPin[256];
        uint32_t maxRedirectionEntry;
    };
}

#endif
