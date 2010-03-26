#include <arch/hal.h>
#include <kernel/IO/Monitor.h>
#include <kernel/global.h>
#include <kernel/multiboot.h>
#include <kernel/utils/DebuggingSymbols.h>
#include <arch/PageFaultHandler.h>
#include <kernel/Memory/PlacementAllocator.h>
#include <arch/interrupts.h>
#include <kernel/IInterruptServiceRoutine.h>
#include <kernel/Time/TimerManager.h>
#include <kernel/Time/Timer.h>
#include <kernel/Memory/Stack.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <kernel/Memory/Virtual/VirtualMemorySpace.h>
#include <arch/ExceptionHandler.h>
#include <kernel/Time/TimerHandler.h>
#include <arch/KeyboardHandler.h>
#include <arch/scheduling.h>

extern      Address             placement;          //defined in linker script
#define     PLACEMENT_SIZE      0x10000             //64KByte

extern      Address             bootStack;          //defined in start.S
#define     STACK_ADDRESS       0xFFC00000          //Uppermost address we can use
#define     STACK_SIZE          0x10000             //64KByte

#define     BIOS_ADDRESS        0x0
#define     BIOS_SIZE           0x10000

using namespace Arch;
using namespace Kernel;
using namespace Memory;
using namespace Processes;
using namespace Time;

void setupKernelMemRegions(Multiboot* m, VirtualMemorySpace* vm);
void attachExceptionHandlers(InterruptDispatcher* irqD);

int main(MultibootInfo* multibootInfo)
{   
    //Prepare monitor output
    kdbg.Clear();
    MAIN_DEBUG_MSG("GeexOS Kernel booting...");
    
    //Get information about environment from GRUB
    MAIN_DEBUG_MSG("Multiboot structure is at " << hex << (unsigned)multibootInfo);
    Multiboot m = Multiboot(multibootInfo);
    
    //Flush GDT and initialize IDT (Interrupts)
    InitializeCPU();
    MAIN_DEBUG_MSG("CPU and Interrupt tables initialized...");
    
    //Initialize Memory
    VirtualMemoryManager::GetInstance()->Init(m.GetLowerMemory() + m.GetUpperMemory());
    
    //Build virtual memory space for kernel ELF
    VirtualMemoryManager::GetInstance()->KernelSpace(new VirtualMemorySpace(VirtualMemoryManager::GetInstance(), "KernelSpace"));
    setupKernelMemRegions(&m, VirtualMemoryManager::GetInstance()->KernelSpace());
    
    //Init symtab and strtab for stacktraces
    //FIXME: Somehow get those strintables accessible again
    //Debug::stringTable = m.elfInfo->GetSection(".strtab");
    //Debug::symbolTable = m.elfInfo->GetSection(".symtab");
    
    //mm->KernelSpace()->DumpRegions(kdbg);
    
    MAIN_DEBUG_MSG("Multiboot structure address: " << hex << (unsigned)m.GetAddress());
    MAIN_DEBUG_MSG("Kernel commandline: " << m.GetKernelCommandline());
    
    InterruptDispatcher* irqD = InterruptDispatcher::GetInstance();
    attachExceptionHandlers(irqD);
    MAIN_DEBUG_MSG("Interrupt dispatcher initialized...");
    
    //Init timer
    InitializeTimer();
    TimerManager *tm = new TimerManager(&Arch::ClockSource);
    irqD->RegisterHandler(IRQ_TIMER, new TimerHandler(tm));
    MAIN_DEBUG_MSG("Timer initialized...");
    
    Arch::EnableInterrupts();
    MAIN_DEBUG_MSG("Interrupts enabled...");
    
    //Make it crash
    //int* a = (int*)0x100000;
    //*a = 0x41414141;
    
    irqD->RegisterHandler(IRQ_KEYBOARD, new KeyboardHandler());
    Arch::UnmaskIRQ(IRQ_KEYBOARD);
    
    MAIN_DEBUG_MSG("Placement pointer is at " << hex << getPlacementPointer());
    PANIC("abc");
    for(;;) {
        //scheduler->Schedule();
        asm volatile("hlt"); //halt cpu until next irq (timer etc.) to switch to next time slice
    }
    
    return 0; 
}

void setupKernelMemRegions(Multiboot* m, VirtualMemorySpace* vm)
{
    Elf32SectionHeader *text = m->elfInfo->GetSection(".text");
    VirtualMemoryRegion* textRegion = new VirtualMemoryRegion(((Address)text->addr) & IDENTITY_POSITION, (size_t)text->size, ".text");
    vm->SetFlags(textRegion, ALLOCFLAG_EXECUTABLE);
    vm->AddRegion(textRegion);
    
    Elf32SectionHeader *data = m->elfInfo->GetSection(".data");
    VirtualMemoryRegion* dataRegion = new VirtualMemoryRegion((Address)data->addr, (size_t)data->size, ".data");
    vm->SetFlags(dataRegion, ALLOCFLAG_WRITABLE);
    vm->AddRegion(dataRegion);
    
    Elf32SectionHeader *rodata = m->elfInfo->GetSection(".rodata");
    VirtualMemoryRegion* rodataRegion = new VirtualMemoryRegion((Address)rodata->addr, (size_t)rodata->size, ".rodata");
    vm->SetFlags(rodataRegion, ALLOCFLAG_NONE);
    vm->AddRegion(rodataRegion);
    
    Elf32SectionHeader *bss = m->elfInfo->GetSection(".bss");
    VirtualMemoryRegion* bssRegion = new VirtualMemoryRegion((Address)bss->addr, (size_t)bss->size, ".bss");
    vm->SetFlags(bssRegion, ALLOCFLAG_WRITABLE);
    vm->AddRegion(bssRegion);
    
    VirtualMemoryRegion* biosRegion = new VirtualMemoryRegion(BIOS_ADDRESS, BIOS_SIZE, "BIOS");
    vm->SetFlags(biosRegion, ALLOCFLAG_WRITABLE);
    vm->AddRegion(biosRegion);
    
    //Create region for placement allocation
    VirtualMemoryRegion* placementRegion = new VirtualMemoryRegion((Address)&placement, PLACEMENT_SIZE, "Placement region");
    vm->SetFlags(placementRegion, ALLOCFLAG_WRITABLE);
    vm->AddRegion(placementRegion);
    
    //Create region for multiboot stuff
    VirtualMemoryRegion* multibootRegion = new VirtualMemoryRegion(m->GetAddress() & IDENTITY_POSITION, 2*PAGE_SIZE, "Multiboot information");
    vm->SetFlags(multibootRegion, ALLOCFLAG_NONE);
    vm->AddRegion(multibootRegion);
    
    //Create region for video memory
    //TODO: Implement proper framebuffer, configure VGA properly and throw this away
    VirtualMemoryRegion* videoRegion = new VirtualMemoryRegion(0xB8000, 2*PAGE_SIZE, "Video memory");
    vm->SetFlags(videoRegion, ALLOCFLAG_WRITABLE);
    vm->AddRegion(videoRegion);
    
    //Create defined Stack and move boot stack to new position
    vm->Allocate(STACK_ADDRESS - STACK_SIZE, STACK_SIZE, "Kernel stack", ALLOCFLAG_WRITABLE);
    Stack *kernelStack = new Stack(STACK_ADDRESS - STACK_SIZE, STACK_SIZE);
    kernelStack->MoveCurrentStackHere((Address)&bootStack);
    VirtualMemoryManager::GetInstance()->KernelStack(kernelStack);
    MAIN_DEBUG_MSG("Stack seems to be successfully moved to defined address: " << hex << STACK_ADDRESS << " with size: " << STACK_SIZE);
    MAIN_DEBUG_MSG("New Stackpointer: " << (unsigned)readStackPointer());
    
    //Whilst initialization of the paging, we allocated the lowermost 4MB for our purposes.
    //Now, that we arranged all our needs regarding to memory with the help of our marvellous
    //virtual memory management, we don't need the whole 4MB.
    //What we do now is a major cleanup. We need to synchronize the regions which are really allocated
    //and needed within our virtual memory space with the paging.
    //Sine we only have a kernel space at this time, we don't bother about other spaces. There are simply none.
    for(Address i = 0x0; i < 4*1024*1024; i += PAGE_SIZE)
    {
        if(vm->FindRegionEnclosingAddress(i) == NULL)
        {
            Address physicalAddr = Paging::GetInstance()->GetPhysicalAddress(i);
            //MAIN_DEBUG_MSG("Virtual address " << hex << (unsigned)i << " pointing to physical " << (unsigned)physicalAddr << " doesn't seem to contain a region.");
            
            Paging::GetInstance()->UnmapAddress(i);
            VirtualMemoryManager::GetInstance()->PhysicalAllocator()->DeallocateFrame(physicalAddr);
        }
    }
}

void attachExceptionHandlers(InterruptDispatcher* irqD)
{
    irqD->RegisterHandler(14, new PageFaultHandler());
    irqD->RegisterHandler(6, new InvalidOpcodeHandler());
    ExceptionHandler *ex = new ExceptionHandler();
    irqD->RegisterHandler(0, ex);
    irqD->RegisterHandler(1, ex);
    irqD->RegisterHandler(2, ex);
    irqD->RegisterHandler(3, ex);
    irqD->RegisterHandler(4, ex);
    irqD->RegisterHandler(5, ex);
    irqD->RegisterHandler(7, ex);
    irqD->RegisterHandler(8, ex);
    irqD->RegisterHandler(9, ex);
    irqD->RegisterHandler(10, ex);
    irqD->RegisterHandler(11, ex);
    irqD->RegisterHandler(12, ex);
    irqD->RegisterHandler(13, ex);
    irqD->RegisterHandler(15, ex);
    irqD->RegisterHandler(16, ex);
}
