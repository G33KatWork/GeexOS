#ifndef _ARCH_LAPIC_H
#define _ARCH_LAPIC_H

#include <arch/types.h>

#include <halinterface/BaseTimer.h>

#define     LAPIC_ENABLE                0x100

// LAPIC Registers
#define		LAPIC_ID            		0x020
#define     LAPIC_VERSION               0x030
#define     LAPIC_TASK_PRIORITY         0x080
#define     LAPIC_ARBITRATION_PRIORITY  0x090
#define     LAPIC_PROCESSOR_PRIORITY    0x0A0
#define     LAPIC_EOI                   0x0B0
#define     LAPIC_REMOTE_READ           0x0C0
#define     LAPIC_LOGICAL_DESTINATION   0x0D0
#define     LAPIC_DESTINATION_FORMAT    0x0E0
#define     LAPIC_SPURIOUS_INT_VECTOR   0x0F0

// LVT Registers
#define     LAPIC_LVT_TIMER             0x320
#define     LAPIC_LVT_THERMAL           0x330
#define     LAPIC_LVT_PERFORMANCE       0x340
#define     LAPIC_LVT_LINT0             0x350
#define     LAPIC_LVT_LINT1             0x360
#define     LAPIC_LVT_ERROR             0x370
#define     LAPIC_LVT_CMCI              0x2F0

// R/W LVT Flags
#define     LAPIC_LVT_TIMERMODE_PERIODIC    (1 << 17)
#define     LAPIC_LVT_TIMERMODE_ONESHOT     (0 << 17)
#define     LAPIC_LVT_MASK                  (1 << 16)
#define     LAPIC_LVT_UNMASK                (0 << 16)
#define     LAPIC_LVT_TRIGGER_LEVEL         (1 << 15)
#define     LAPIC_LVT_TRIGGER_EDGE          (0 << 15)
#define     LAPIC_LVT_PINPOLARITY_LOW       (1 << 13)
#define     LAPIC_LVT_PINPOLARITY_HIGH      (0 << 13)

// Timer Registers
#define     LAPIC_TIMER_DIVIDER         0x3E0
#define     LAPIC_TIMER_INITIALCOUNT    0x380
#define     LAPIC_TIMER_CURRENTCOUNT    0x390

// Clock divider
#define     LAPIC_TIMER_DIVIDE_1        0xB
#define     LAPIC_TIMER_DIVIDE_2        0x0
#define     LAPIC_TIMER_DIVIDE_4        0x1
#define     LAPIC_TIMER_DIVIDE_8        0x2
#define     LAPIC_TIMER_DIVIDE_16       0x3
#define     LAPIC_TIMER_DIVIDE_32       0x8
#define     LAPIC_TIMER_DIVIDE_64       0x9
#define     LAPIC_TIMER_DIVIDE_128      0xA


// R/O LVT Flags (to be used as masks)
#define     LAPIC_LVT_REMOTEIRR             (1 << 14)
#define     LAPIC_LVT_DELIVERYSTATUS        (1 << 12)

#define     LAPIC_LVT_DELIVERY_FIXED        (0 << 8)    //000
#define     LAPIC_LVT_DELIVERY_SMI          (2 << 8)    //010
#define     LAPIC_LVT_DELIVERY_NMI          (4 << 8)    //100
#define     LAPIC_LVT_DELIVERY_INIT         (5 << 8)    //101
#define     LAPIC_LVT_DELIVERY_EXTINT       (7 << 8)    //111

namespace Arch
{
    /**
     * The LAPIC is the local advanced programmable interrupt controller
     * It contains a timer and a local interrupt controller for each CPU.
     * It doesn't inherit from BaseInterruptController, because it is not
     * a general purpose hardware interrupt controller. This is the IOAPIC.
    **/
    class LAPIC : public BaseTimer
    {
    public:
        LAPIC() {}
        
        void MapIntoIOMemory();
        void Initialize();
        void DetermineBusFrequency();
        
        uint32_t GetID(void) { return (LAPICRead(LAPIC_ID) & 0xffffffff) >> 24; }
        
        void MaskLocalVector(Address lvtOffset);
        void UnmaskLocalVector(Address lvtOffset);
        
        inline void LocalEndOfInterrupt(void) { LAPICWrite(LAPIC_EOI, 0); }
        
        static bool IsAvailable();
        
        //BaseTimer Interface
        virtual const char* GetName() { return "LAPIC"; }
        
        virtual unsigned long GetTickLengthUs() { return curTickLen; }
        virtual void SetTickLengthUs(unsigned long us);
        
        virtual ClockType GetClockType() { return CLKTYPE_PERIODIC; }
        
        virtual void WaitForTick();
        
        virtual void EnableInterrupt() { UnmaskLocalVector(LAPIC_LVT_TIMER); }
        virtual void DisableInterrupt() { MaskLocalVector(LAPIC_LVT_TIMER); }
        
    private:
        Address GetBaseAddress();
        
        inline uint32_t LAPICRead(Address offset) { return *(volatile uint32_t *)((char*)GetBaseAddress() + offset); }
        inline void LAPICWrite(Address offset, uint32_t data) { *(volatile uint32_t *)((char *)GetBaseAddress() + offset) = data; }
        
        unsigned long curTickLen;
        
        static uint64_t lapicBusFrequency;
    };
}

#endif
