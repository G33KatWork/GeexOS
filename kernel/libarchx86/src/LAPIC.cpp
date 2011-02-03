#include <arch/internal/LAPIC.h>
#include <arch/internal/CPUID.h>
#include <arch/internal/MSR.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>

using namespace Arch;
using namespace Memory;

#define     CALIBRATIONLOOP_FREQUENCY   100
#define     CALIBRATIONLOOP_COUNT       4

uint64_t LAPIC::lapicBusFrequency = 0;

void LAPIC::MapIntoIOMemory()
{
    Address a, d;
    getMSR(MSR_APIC_BASE, &a, &d);
    Address phys = a & 0xFFFFF000;
    
    VirtualMemoryManager::GetInstance()->PhysicalAllocator()->MarkAsUsed(phys);
    VirtualMemoryManager::GetInstance()->IOMemory()->MapPhysical(phys, 0x1000, "Local APIC");
}

void LAPIC::Initialize()
{
    curTickLen = 0;
    
    //Set spurious interrupt vector to 0xff
    uint32_t conf = LAPICRead(LAPIC_SPURIOUS_INT_VECTOR) & 0xffffff00;
    conf |= LAPIC_ENABLE | 0xff;
    LAPICWrite(LAPIC_SPURIOUS_INT_VECTOR, conf);
    
    //TODO: Install spurious irq handler
    
    //FIXME: LINT0 is level triggered high, is this somewhere in ACPI available?
    //extint for legacy PIC on LINT0
    conf = LAPIC_LVT_PINPOLARITY_HIGH | LAPIC_LVT_TRIGGER_LEVEL | LAPIC_LVT_DELIVERY_EXTINT;
    LAPICWrite(LAPIC_LVT_LINT0, conf);
    //nmi on LINT1
    conf = LAPIC_LVT_PINPOLARITY_HIGH | LAPIC_LVT_TRIGGER_LEVEL | LAPIC_LVT_DELIVERY_NMI;
    LAPICWrite(LAPIC_LVT_LINT1, conf);
    
    /*DEBUG_MSG("APIC LAPIC_LVT_TIMER: " << Debug::hex << LAPICRead(LAPIC_LVT_TIMER));
    DEBUG_MSG("APIC LAPIC_LVT_THERMAL: " << Debug::hex << LAPICRead(LAPIC_LVT_THERMAL));
    DEBUG_MSG("APIC LAPIC_LVT_PERFORMANCE: " << Debug::hex << LAPICRead(LAPIC_LVT_PERFORMANCE));
    DEBUG_MSG("APIC LAPIC_LVT_ERROR: " << Debug::hex << LAPICRead(LAPIC_LVT_ERROR));
    DEBUG_MSG("APIC LAPIC_LVT_CMCI: " << Debug::hex << LAPICRead(LAPIC_LVT_CMCI));
    DEBUG_MSG("APIC LAPIC_TASK_PRIORITY: " << Debug::hex << LAPICRead(LAPIC_TASK_PRIORITY));*/
    
    //set timer clock divider to 1
    conf = LAPICRead(LAPIC_TIMER_DIVIDER) & 0xfffffff0;
    conf |= LAPIC_TIMER_DIVIDE_1;
    LAPICWrite(LAPIC_TIMER_DIVIDER, conf);
    
    //Set timer interrupt vector
    conf = LAPICRead(LAPIC_LVT_TIMER) & 0xfffdff00;
    conf |= LAPIC_LVT_TIMERMODE_PERIODIC;
    conf |= 0xfb;     //FIXME: Get interrupt vector number from somewhere in the HAL
    LAPICWrite(LAPIC_LVT_TIMER, conf);
    
    //Mask timer interrupt
    conf = LAPICRead(LAPIC_LVT_TIMER);
    conf |= LAPIC_LVT_MASK;
    LAPICWrite(LAPIC_LVT_TIMER, conf);
    
    //Accept all interrupts for now -> Priority = 0
    LAPICWrite(LAPIC_TASK_PRIORITY, 0);
    
    //Set destination format to flat model
    LAPICWrite(LAPIC_DESTINATION_FORMAT, 0xF0000000);
    
    //Set LAPIC global enable bit
    uint32_t a, d;
    getMSR(MSR_APIC_BASE, &a, &d);
    a |= (1 << 11);
    setMSR(MSR_APIC_BASE, a, d);
}

void LAPIC::MaskLocalVector(Address lvtOffset)
{
    uint32_t conf = LAPICRead(lvtOffset);
    conf |= LAPIC_LVT_MASK;
    LAPICWrite(lvtOffset, conf);
}

void LAPIC::UnmaskLocalVector(Address lvtOffset)
{
    uint32_t conf = LAPICRead(lvtOffset);
    DEBUG_MSG("conf: " << Debug::hex << conf);
    conf &= ~(LAPIC_LVT_MASK);
    DEBUG_MSG("conf: " << Debug::hex << conf);
    LAPICWrite(lvtOffset, conf);
}

bool LAPIC::IsAvailable()
{
    uint32_t a, d;
    cpuid(CPUID_GETFEATURES, &a, &d);
    return d & CPUID_FLAG_APIC;
}

Address LAPIC::GetVirtBaseAddress()
{
    Address a, d;
    getMSR(MSR_APIC_BASE, &a, &d);
    Address phys = a & 0xFFFFF000;
    
    Address virtMapped = VirtualMemoryManager::GetInstance()->IOMemory()->TranslatePhysicalAddress(phys);
    if(!virtMapped)
        PANIC("Physical LAPIC address couldn't be translated to IO Memory Space");
    
    return virtMapped;
}

void LAPIC::DetermineBusFrequency()
{
    //This is to be called after Initialize(). On an SMP System it only needs to be done once.
    //Initialize() will mask the timer interrupt vector and set the clock divider to 1. 
    
    //Create a local PIT object, just to configure it for our calibration purposes
    PIT calibrationPit = PIT();
    calibrationPit.SetTickLengthUs(1.0 / CALIBRATIONLOOP_FREQUENCY * 1000 * 1000); //100Hz = 10ms
    
    //wait for the next timer tick to be sure that we get a full tick after that
    calibrationPit.WaitForTick();
    
    //load maximum value into LAPIC timer
    LAPICWrite(LAPIC_TIMER_INITIALCOUNT, 0xffffffff);
    
    //wait for 4 ticks = 40ms
    for(int i = 0; i < CALIBRATIONLOOP_COUNT; i++)
        calibrationPit.WaitForTick();
    
    //read current LAPIC timer value
    uint32_t count = LAPICRead(LAPIC_TIMER_CURRENTCOUNT);
    
    count = 0xffffffff - count;
    
    //DEBUG_MSG("count: " << Debug::dec << count);
    
    uint64_t ticksPerSec = (uint64_t)(count * CALIBRATIONLOOP_FREQUENCY / CALIBRATIONLOOP_COUNT);
    lapicBusFrequency = ticksPerSec;
}

void LAPIC::SetTickLengthUs(unsigned long us)
{
    curTickLen = us;
    uint32_t reloadValue = lapicBusFrequency * (us / 1000.0 / 1000.0);
    LAPICWrite(LAPIC_TIMER_INITIALCOUNT, reloadValue);
}

void LAPIC::WaitForTick()
{
    uint32_t curCount, prevCount;
    curCount = LAPICRead(LAPIC_TIMER_CURRENTCOUNT);
    do {
        prevCount = curCount;
        curCount = LAPICRead(LAPIC_TIMER_CURRENTCOUNT);
    } while(prevCount >= curCount);
}

/*void Arch::LAPICSetTimerVector(uint8_t vectorNumber, bool periodic)
{
    uint32_t conf = LAPICRead(LAPIC_LVT_TIMER) & 0xfffdff00;
    
    if(periodic)
        conf |= LAPIC_LVT_TIMERMODE_PERIODIC;
    else
        conf &= ~(LAPIC_LVT_TIMERMODE_PERIODIC);
    
    conf |= vectorNumber;
    
    LAPICWrite(LAPIC_LVT_TIMER, conf);
}

void Arch::LAPICSetTimerFrequency(uint32_t tickFrequency)
{
    uint32_t reloadVal = lapicTimerFrequency / tickFrequency;
    LAPICWrite(LAPIC_TIMER_INITIALCOUNT, reloadVal);
}*
*/
