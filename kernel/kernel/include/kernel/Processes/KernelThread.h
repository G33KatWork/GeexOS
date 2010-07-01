#ifndef PROCESSES_KERNELTHREAD_H_
#define PROCESSES_KERNELTHREAD_H_

#include <types.h>
#include <kernel/Processes/Thread.h>
#include <kernel/Processes/KernelThread.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>

namespace Processes
{
    class KernelThread : public Thread
    {
    public:
        KernelThread(unsigned int threadId, void(*entryFunction)(int), int arg, size_t stackSize, const char* threadName);
        ~KernelThread();
    
    private:
        Memory::VirtualMemoryRegion* threadStack;
    };
}

#endif
