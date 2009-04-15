#include <kernel/IO/Monitor.h>
#include <kernel/global.h>

using namespace IO;

int gp;

int main(void)
{
    kout.Clear();
    DEBUG_MSG("I'm a DEBUG message!");
    
    //PANIC("bla");
    //ASSERT(1 < 0);
        
    return 0; 
}
