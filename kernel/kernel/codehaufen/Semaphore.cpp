#include <kernel/Processes/Semaphore.h>
#include <arch/hal.h>

using namespace Arch;
using namespace Processes;

void Semaphore::up(semaphore* sem)
{
    (*sem)++;
    return;
}

int Semaphore::down(semaphore* sem)
{
    //FIXME: Use real locking stuff and don't use Enable/DisableInterrupts
    DisableInterrupts();

    if (*sem) {
        (*sem)--;
        EnableInterrupts();
        return 0;
    } else {
        EnableInterrupts();
        return -1;
    }
}