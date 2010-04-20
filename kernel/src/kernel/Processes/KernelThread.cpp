#include <kernel/Processes/KernelThread.h>
#include <lib/string.h>
#include <arch/AddressLayout.h>

using namespace Processes;
using namespace Memory;

KernelThread::KernelThread(unsigned int threadId, void(*entryFunction)(int), int arg, size_t stackSize, const char* threadName)
    : Thread(threadId, (Address)entryFunction, 0, 0, threadName, false)
{
    threadStack = VirtualMemoryManager::GetInstance()->KernelSpace()->AllocateInRange(KERNEL_THREAD_STACK_REGION_START, KERNEL_THREAD_STACK_REGION_END, stackSize, threadName, ALLOCFLAG_WRITABLE);
    ASSERT(threadStack != NULL, "Returned Stack MemoryRegion for a KernelThread was NULL");
    
    /* We are building this stack here:
     *                     ...
     *          |                        |
     *          | Stackframe of Caller   |  <-- Doesn't exist for now
     *          |------------------------|
     *          |       Parameter        |
     *          |------------------------|
     *          |     Return Address     |  <-- Fake return address for now
     *  ESP --> |------------------------|
     *          | $foo of called function|  <-- Pushed EBP, local variables etc.
     *          |                        |
     *                     ...
     */
    
    memset((void*)threadStack->StartAddress(), 0, stackSize);
    int* stack = (int*)threadStack->StartAddress();
    stack[stackSize/sizeof(int) - 1] = arg; //Argument
    stack[stackSize/sizeof(int) - 2] = 0;   //Return Address
    
    this->SetStackPointer(threadStack->StartAddress() + stackSize - 2*sizeof(int));
    this->SetBasePointer(0);
}

KernelThread::~KernelThread()
{
    VirtualMemoryManager::GetInstance()->KernelSpace()->Deallocate(threadStack);
}
