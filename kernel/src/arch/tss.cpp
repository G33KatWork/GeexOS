#include <lib/types.h>
#include <arch/tss.h>

using namespace Arch;

TssEntry tssEntry;

void Arch::tss_set_kernel_stack(Address stack)
{
    tssEntry.esp0 = stack;
}
