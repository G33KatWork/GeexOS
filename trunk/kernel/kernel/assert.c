#include <stdint.h>
#include "assert.h"
#include "panic.h"

void panic_assert(const char *file, unsigned line, const char *desc)
{
	kernel_panic("Assertion failed at %s:%u: %s", file, line, desc);
}
