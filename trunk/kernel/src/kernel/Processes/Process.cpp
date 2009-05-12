#include <kernel/Processes/Process.h>

using namespace Processes;

Process::Process(unsigned int id, unsigned int sp, PageDirectory* pd)
{
    this->pid = id;
    this->stackPointer = sp;
    this->page_directory = pd;
    
    this->lastRun = 0;
}
