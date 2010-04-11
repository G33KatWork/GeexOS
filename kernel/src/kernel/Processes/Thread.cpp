#include <kernel/Processes/Thread.h>
#include <arch/scheduling.h>
#include <kernel/debug.h>
#include <lib/string.h>

using namespace Arch;
using namespace Processes;

static inline uint32_t getEflags()
{
    uint32_t eflags;
    asm volatile("         \
      pushf;               \
      pop %0;           "
      : "=r"(eflags));
    
    return eflags;
}

Thread::Thread(unsigned int id, Address initialIP, Address initialSP, Address initialBP, const char* threadName/*, PageDirectory* pd*/)
{
    memset(&threadInfo, 0, sizeof(ThreadInfo));
    
    tid = id;
    threadInfo.eip = initialIP;
    threadInfo.esp = initialSP;
    threadInfo.ebp = initialBP;
    name = threadName;
    
    //FIXME: remove hardcoded stuff here
    threadInfo.cs = 0x8;
    threadInfo.ds = 0x10;
    threadInfo.es = 0x10;
    threadInfo.fs = 0x10;
    threadInfo.gs = 0x10;
    threadInfo.ss = 0x10;
    
    threadInfo.eflags = getEflags();
    /*page_directory = pd;
    priority = 1;*/
}

void Thread::Sleep()
{
    
}

void Thread::Wakeup()
{
    
}
