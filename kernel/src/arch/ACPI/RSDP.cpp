#include <arch/ACPI/RSDP.h>
#include <kernel/debug.h>
#include <kernel/global.h>
#include <lib/string.h>
#include <arch/AddressLayout.h>

using namespace Arch::ACPI;
using namespace IO;

RSDP::RSDP()
{
    descriptor = NULL;
    
    Address ebda = *(unsigned short *)(0x040E + KERNEL_BASE);
    ebda = (Address)((ebda << 4) + KERNEL_BASE);
    HAL_ACPI_DEBUG_MSG("BIOS EBDA at " << hex << ebda);
    
    if(!Find(0xF0000 + KERNEL_BASE, 0x10000))
        Find(ebda, 0x400);
}

bool RSDP::Find(Address start, size_t size)
{
    HAL_ACPI_DEBUG_MSG("Scanning address " << hex << start << " with size " << size << " for RSDP");
    
    Address curAddr = start;
    
    while(size > 0)
    {
        RSDPDescriptor* d = (RSDPDescriptor*)curAddr;
        if(memcmp("RSD PTR ", d->Signature, 8) == 0)
        {
            HAL_ACPI_DEBUG_MSG("Found RSDP at " << hex << curAddr);
            descriptor = d;
            return true;
        }
        
        curAddr += 16;
        size -= 16;
    }
    
    return false;
}

bool RSDP::ChecksumValidInternal(Address start, size_t len)
{
    char checksum = 0;
    char* ptr = (char*)start;
    for(unsigned int i = 0; i < len; i++)
        checksum += ptr[i];
    
    return checksum == 0;
}

bool RSDP::IsValid()
{
    if(descriptor == NULL)
        return false;
    
    if(!ChecksumValidInternal((Address)descriptor, RSDPLENGTH_V1))
        return false;
    
    if(descriptor->Revision > 0)
    {
        if(!ChecksumValidInternal((Address)descriptor, RSDPLENGTH_V2))
            return false;
    }
    
    return true;
}
