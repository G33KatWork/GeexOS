#include <kernel/Processes/Thread.h>
#include <arch/HAL.h>
#include <kernel/debug.h>
#include <string.h>

using namespace Arch;
using namespace Processes;

Thread::Thread(unsigned int threadId, Address initialIP, Address initialSP, Address initialBP, const char* threadName, bool umode, BasePageDirectory* pd)
{
    tid = threadId;
    name = threadName;
    state = THREAD_RUNNING;
    usermode = umode;
    
    if(umode)
        threadContext = ThreadContext::BuildForUsermode(initialIP, initialSP, initialBP, pd);
    else
        threadContext = ThreadContext::BuildForKernelmode(initialIP, initialSP, initialBP, pd);
    
    //initializeThreadInfoForKernel(&threadInfo, initialIP, initialSP, initialBP, pd);
    
    /*page_directory = pd;
    priority = 1;*/
}
