#include <kernel/Processes/Thread.h>
#include <arch/scheduling.h>
#include <kernel/debug.h>
#include <lib/string.h>

using namespace Arch;
using namespace Processes;

Thread::Thread(unsigned int threadId, Address initialIP, Address initialSP, Address initialBP, const char* threadName, bool umode/*, PageDirectory* pd*/)
{
    tid = threadId;
    name = threadName;
    state = THREAD_RUNNING;
    usermode = umode;
    
    initializeThreadInfoForKernel(&threadInfo, initialIP, initialSP, initialBP);
    
    /*page_directory = pd;
    priority = 1;*/
}
