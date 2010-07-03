#ifndef PROCESSES_KERNELTHREAD_H_
#define PROCESSES_KERNELTHREAD_H_

#include <types.h>
#include <kernel/Processes/Thread.h>
#include <kernel/Processes/KernelThread.h>
#include <kernel/Memory/Virtual/Regions/KernelThreadStackRegion.h>

namespace Processes
{
    class KernelThread : public Thread
    {
    public:
        KernelThread(unsigned int threadId, void(*entryFunction)(int), int arg, size_t intialStackSize, size_t maxStackSize, const char* threadName);
        ~KernelThread();
    
    private:
        Memory::KernelThreadStack* threadStack;
    };
}

#endif
