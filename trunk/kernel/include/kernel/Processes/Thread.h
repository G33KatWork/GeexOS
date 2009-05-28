#ifndef PROCESSES_THREAD_H_
#define PROCESSES_THREAD_H_

#include <lib/types.h>
#include <kernel/Memory/Paging.h>
#include <arch/hal.h>
#include <arch/CPUContext.h>

using namespace Memory;
using namespace Arch;

#include <kernel/global.h>
using namespace IO;

namespace Processes
{
    class Thread
    {
    public:
        Thread(void (*func)(void*), void *arg, unsigned int id, PageDirectory* pd)
        {
            tid = id;
            page_directory = pd;
            context = new CPUContext(func, arg);
            priority = 1;
        }
        
        ~Thread()
        {
            delete context;
        }
        
        int GetId() { return tid; }
        PageDirectory* GetPageDirectory() { return page_directory; }
        void SetPageDirectory(PageDirectory* p) { page_directory = p; }
        CPUContext* GetContext(){ return context; }
        int GetPriority() { return priority; }
        void SetPriority(unsigned char p) { priority = p; }
        unsigned long GetTimeslice() { return timeslice; }
        void SetTimeslice(unsigned long t) { timeslice = t; }
        
        void Sleep();
        void Wakeup();
        
        
    private:
        int tid;
        unsigned char priority;
        unsigned long timeslice;
        CPUContext *context;
        PageDirectory* page_directory;
    };
}
#endif
