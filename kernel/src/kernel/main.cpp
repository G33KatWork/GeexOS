#include <arch/hal.h>
#include <kernel/IO/Monitor.h>
#include <kernel/global.h>
#include <kernel/utils/multiboot.h>
#include <arch/PageFaultHandler.h>
#include <kernel/Memory/PlacementAllocator.h>
#include <arch/interrupts.h>
#include <kernel/IInterruptServiceRoutine.h>
#include <kernel/Time/TimerManager.h>
#include <kernel/Time/Timer.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <kernel/Memory/Virtual/VirtualMemorySpace.h>
#include <arch/ExceptionHandler.h>
#include <kernel/Time/TimerHandler.h>
#include <arch/KeyboardHandler.h>
#include <arch/scheduling.h>
#include <kernel/utils/ElfInformation.h>
#include <kernel/Processes/Scheduler.h>
#include <kernel/Processes/KernelThread.h>
#include <arch/AddressLayout.h>
#include <kernel/Memory/Virtual/Regions/KernelStackMemoryRegion.h>

#include <lib/string.h>

#include <kernel/Memory/Virtual/Regions/PagedMemoryRegion.h>

extern      Address             bootStack;          //defined in start.S


using namespace Arch;
using namespace Kernel;
using namespace Memory;
using namespace Processes;
using namespace Time;

void syncMemregionsWithPaging(void);
void attachExceptionHandlers(InterruptDispatcher* irqD);

void umode(int UNUSED(arg))
{
    asm volatile("  \
         cli; \
         movw $0x23, %ax; \
         movw %ax, %ds; \
         movw %ax, %es; \
         movw %ax, %fs; \
         movw %ax, %gs; \
                       \
         movl %esp, %eax; \
         pushl $0x23; \
         pushl %eax; \
         pushf; \
         pop %eax; \
         or $0x200, %eax; \
         push %eax; \
         pushl $0x1B; \
         pushl $1f; \
         iret; \
       1: \
         ");
     while(1);
}

void foo(int arg)
{   
    while(1)
    {
        kdbg.PrintChar((char)arg);
        for(int i = 0; i < 10000000; i++);
    }
}

int main(MultibootInfo* multibootInfo)
{   
    //Prepare monitor output
    kdbg.Clear();
    MAIN_DEBUG_MSG("GeexOS Kernel booting...");
    
    char cpuVendor[17] = {0};
    GetCPUVendor(cpuVendor);
    MAIN_DEBUG_MSG("CPU Vendor: " << cpuVendor);
    
    //Flush GDT and initialize IDT (Interrupts)
    InitializeCPU();
    MAIN_DEBUG_MSG("CPU and Interrupt tables initialized...");
    
    //Get information about environment from GRUB
    Multiboot m = Multiboot(multibootInfo);
    
    InitDone();
    
    //Initialize Memory
    VirtualMemoryManager::GetInstance()->Init(m.GetLowerMemory() + m.GetUpperMemory());
    
    //Build virtual memory space for kernel ELF
    VirtualMemoryManager::GetInstance()->KernelSpace(new VirtualMemorySpace(VirtualMemoryManager::GetInstance(), "KernelSpace"));
    
    //Parse ELF-Stuff delivered from GRUB and create .text, .data, .rodata, .bss and .placement sections in kernel space
    ElfInformation* elfInfo = new ElfInformation(m.GetELFAddress(), m.GetELFshndx(), m.GetELFSize(), m.GetELFNum());
    VirtualMemoryManager::GetInstance()->KernelElf(elfInfo);
    
    //Create Arch-specific memory regions in kernel space
    //On x86: Framebuffer for textmode and lowest 64K for BIOS
    SetupArchMemRegions(&m);
    
    //Create defined Stack and move boot stack to new position
    KernelStackMemoryRegion* kernelStack = new KernelStackMemoryRegion(KERNEL_STACK_ADDRESS - KERNEL_STACK_SIZE, KERNEL_STACK_SIZE, 0x1000, "Main kernel stack");
    kernelStack->MoveCurrentStackHere((Address)&bootStack);
    VirtualMemoryManager::GetInstance()->KernelStack(kernelStack);
    MAIN_DEBUG_MSG("Stack seems to be successfully moved to defined address: " << hex << KERNEL_STACK_ADDRESS << " with size: " << KERNEL_STACK_SIZE);
    MAIN_DEBUG_MSG("New Stackpointer: " << readStackPointer());
    
    syncMemregionsWithPaging();
    
    MAIN_DEBUG_MSG("Multiboot structure address: " << hex << m.GetAddress());
    MAIN_DEBUG_MSG("Kernel commandline: " << m.GetKernelCommandline());
    
    InterruptDispatcher* irqD = InterruptDispatcher::GetInstance();
    attachExceptionHandlers(irqD);
    MAIN_DEBUG_MSG("Interrupt dispatcher initialized...");
    
    //Init timer
    InitializeTimer();
    TimerManager *tm = new TimerManager(&Arch::ClockSource);
    irqD->RegisterHandler(IRQ_TIMER, new TimerHandler(tm, Scheduler::GetInstance()));
    MAIN_DEBUG_MSG("Timer initialized...");
    
    Arch::EnableInterrupts();
    MAIN_DEBUG_MSG("Interrupts enabled...");
    
    /*char line[20];
    SerialConsole *ser = new SerialConsole(SERIAL_COM2);
    ser->ReadLine(line, 20);
    kdbg << "Read line: " << line;    
    int hexbla = ser->ReadHex();
    kdbg << hex << "read: " << hexbla << endl;
    int foo = ser->ReadDec();
    kdbg << dec << "read: " << foo << endl;*/
    
    //Make it crash
    //int* a = (int*)0x100000;
    //*a = 0x41414141;
    
    irqD->RegisterHandler(IRQ_KEYBOARD, new KeyboardHandler());
    Arch::UnmaskIRQ(IRQ_KEYBOARD);
    
    MAIN_DEBUG_MSG("Placement pointer is at " << hex << getPlacementPointer());
    
    VirtualMemoryManager::GetInstance()->KernelSpace()->DumpRegions(kdbg);
    
    //KernelThread* thread = new KernelThread(1, foo, (int)'A', PAGE_SIZE, "A Thread");
    //Scheduler::GetInstance()->AddThread(thread);
    /*KernelThread* thread2 = new KernelThread(2, foo, (int)'C', PAGE_SIZE, "C Thread");
    Scheduler::GetInstance()->AddThread(thread2);
    KernelThread* thread3 = new KernelThread(3, foo, (int)'D', PAGE_SIZE, "D Thread");
    Scheduler::GetInstance()->AddThread(thread3);*/
    
    //VirtualMemoryRegion* uModeCode = VirtualMemoryManager::GetInstance()->KernelSpace()->Allocate(0x3000000, 0x1000, "Usercode", ALLOCFLAG_WRITABLE|ALLOCFLAG_EXECUTABLE|ALLOCFLAG_USERMODE);
    //VirtualMemoryRegion* kstack = VirtualMemoryManager::GetInstance()->KernelSpace()->Allocate(0x4000000, 0x1000, "kstack", ALLOCFLAG_WRITABLE);
    //Arch::gdt_set_kernel_stack(0x4000000+0x1000);
    /*void* codeStart = (void*)uModeCode->StartAddress();
    memcpy(codeStart, (const void*)umode, 0x1000);
    typedef void threadfunc(int);
    threadfunc* func;
    char addr[] = {0x00, 0x00, 0x00, 0x3}; 
    memcpy((void*)&func, addr, 4);
    DEBUG_MSG("func points to " << hex << (unsigned)func);*/
    //KernelThread* thread4 = new KernelThread(4, umode, (int)'U', PAGE_SIZE, "Umode Thread");
    //Scheduler::GetInstance()->AddThread(thread4);
    
    //Initialize the scheduler
    //Scheduler::GetInstance()->SetTimerManager(tm);
    //Scheduler::GetInstance()->DumpThreads(kdbg);
    
    for(;;) {
        kdbg << "B";
        for(int i = 0; i < 10000000; i++);
    }
    
    return 0; 
}

void syncMemregionsWithPaging(void)
{
    //Whilst initialization of the paging, we allocated the lowermost 4MB for our purposes.
    //Now, that we arranged all our needs regarding to memory with the help of our marvellous
    //virtual memory management, we don't need the whole 4MB.
    //What we do now is a major cleanup. We need to synchronize the regions which are really allocated
    //and needed within our virtual memory space with the paging.
    //Sine we only have a kernel space at this time, we don't bother about other spaces. There are simply none.
    for(Address i = KERNEL_BASE; i < 4*1024*1024+KERNEL_BASE; i += PAGE_SIZE)
    {
        if(VirtualMemoryManager::GetInstance()->KernelSpace()->FindRegionEnclosingAddress(i) == NULL)
        {
            Address physicalAddr = Paging::GetInstance()->GetPhysicalAddress(i);
            //MAIN_DEBUG_MSG("Virtual address " << hex << i << " pointing to physical " << physicalAddr << " doesn't seem to contain a region.");
            
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
