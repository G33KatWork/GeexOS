#include <kernel/Processes/Thread.h>
#include <arch/scheduling.h>
#include <kernel/debug.h>
#include <lib/string.h>

using namespace Arch;
using namespace Processes;

Thread::Thread(unsigned int threadId, Address initialIP, Address initialSP, Address initialBP, const char* threadName/*, PageDirectory* pd*/)
{
    tid = threadId;
    name = threadName;
    
    initializeThreadInfoForKernel(&threadInfo, initialIP, initialSP, initialBP);
    
    /*page_directory = pd;
    priority = 1;*/
}

void Thread::Sleep()
{
    
}

void Thread::Wakeup()
{
    
}
