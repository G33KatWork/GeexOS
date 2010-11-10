#include <kernel/Processes/KernelThread.h>
#include <string.h>
#include <arch/AddressLayout.h>
#include <kernel/Memory/Virtual/Regions/KernelThreadStackRegion.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>

using namespace Processes;
using namespace Memory;

KernelThread::KernelThread(unsigned int threadId, void(*entryFunction)(int), int arg, size_t stackSize, const char* threadName)
    : Thread(threadId, (Address)entryFunction, 0, 0, threadName, false, CurrentHAL->GetPaging()->GetKernelDirectory())
{
    threadStack = VirtualMemoryManager::GetInstance()->KernelThreadStacks()->CreateStack(stackSize);
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
    
    //memset((void*)threadStack->Beginning(), 0, stackSize);
    int* stack = (int*)threadStack->GetEndAddress();
    stack[stackSize/sizeof(int) - 1] = arg; //Argument
    stack[stackSize/sizeof(int) - 2] = 0;   //Return Address
    
    this->SetStackPointer(threadStack->GetStartAddress() - 2*sizeof(int));
    this->SetFramePointer(0);
}

KernelThread::~KernelThread()
{
    VirtualMemoryManager::GetInstance()->KernelThreadStacks()->DestroyStack(threadStack);
}
