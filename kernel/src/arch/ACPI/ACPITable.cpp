#include <arch/ACPI/ACPITable.h>
#include <kernel/debug.h>
#include <kernel/global.h>
#include <lib/string.h>

using namespace Arch::ACPI;
using namespace IO;

bool ACPITable::ChecksumValid(Address start, size_t len)
{
    char checksum = 0;
    char* ptr = (char*)start;
    for(unsigned int i = 0; i < len; i++)
        checksum += ptr[i];
    
    return checksum == 0;
}
