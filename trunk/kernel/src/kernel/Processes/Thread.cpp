#include <kernel/Processes/Thread.h>
#include <arch/scheduling.h>

using namespace Arch;
using namespace Processes;

Thread::Thread(unsigned int id, PageDirectory* pd)
{
    tid = id;
    page_directory = pd;
    priority = 1;
}

void Thread::Sleep()
{
    
}

void Thread::Wakeup()
{
    
}

void Thread::SwitchTo()
{
	
}
