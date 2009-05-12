#ifndef PROCESSES_SCHEDULER_H_
#define PROCESSES_SCHEDULER_H_

#include <lib/types.h>
#include <kernel/Processes/Process.h>
#include <kernel/DataStructures/OrderedArray.h>

using namespace DataStructures;

namespace Processes
{
    class Scheduler
    {
    public:
        static Scheduler* GetInstance();
        
        void Schedule();
        void AddProcess(Process* p);
        
    private:
        static Scheduler* instance;
        
        //FIXME: Use dynamic array
        OrderedArray<Process*, 128> *processQueue;
        Process* currentProcess;
        
        Scheduler();
        void switchToProcess(Process* p);
    };
}
#endif
