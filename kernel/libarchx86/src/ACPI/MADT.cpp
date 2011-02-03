#include <arch/internal/ACPI/MADT.h>
#include <string.h>

using namespace Arch::ACPI;

unsigned int MADT::GetAPICStructureCount()
{
	size_t baseSize = ACPITABLEHEADERLEN + 2*sizeof(uint32_t);
	size_t curSize = baseSize;
	size_t structCount = 0;
	Address start = GetAddress();
	
	while(curSize < GetLengthInternal())
	{
		structCount++;
		APICStructureHeader* struc = (APICStructureHeader*)(start + curSize);
		curSize += struc->Length;
	}
	
	return structCount;
}

APICStructureHeader* MADT::GetAPICStruct(unsigned int n, ApicStructureTypes* type)
{
	//ACPI header length and space for LocalAPICAddress and Flags
	//Current offset in memory relative to ACPI Table start which points to the next APICStructureHeader
	size_t curOffset = ACPITABLEHEADERLEN + 2*sizeof(uint32_t);
	
	Address start = GetAddress();
	unsigned int i = 0;
	
	if(n > GetAPICStructureCount())
        return NULL;
	
	while(i < n)
	{
		ASSERT(curOffset < GetLengthInternal(), "Index out of bounds");
		i++;
		APICStructureHeader* struc = (APICStructureHeader*)(start + curOffset);
		curOffset += struc->Length;
	}
	
	APICStructureHeader* retStruct = (APICStructureHeader*)(GetAddress() + curOffset);
	if(type != NULL)
		*type = (ApicStructureTypes)retStruct->Type;
	return retStruct;
}

APICStructureHeader* MADT::GetAPICStructOfType(unsigned int start, ApicStructureTypes type)
{
	//ACPI header length and space fot LocalAPICAddress and Flags
	//Current offset in memory relative to ACPI Table start which points to the next APICStructureHeader
	size_t curOffset = ACPITABLEHEADERLEN + 2*sizeof(uint32_t);
	
	unsigned int i = 0;
	unsigned int totalStructCount = GetAPICStructureCount();
	
	if(start > totalStructCount)
        return NULL;
	
	while(i < totalStructCount)
	{
		APICStructureHeader* struc = (APICStructureHeader*)(GetAddress() + curOffset);
		if(struc->Type == type)
            return struc;
		
		curOffset += struc->Length;
		i++;
	}
	
    return NULL;
}

int MADT::GetAPICStructIndex(APICStructureHeader* apicStruct)
{
    //Sanity check for supplied pointer
    ASSERT(((Address)apicStruct) >= GetAddress() && ((Address)apicStruct) < GetAddress() + GetLengthInternal(), "Previous pointer doesn't point to a valid location in this MADT");
    
    unsigned int i = 0;
    Address start = GetAddress();
    size_t curOffset = ACPITABLEHEADERLEN + 2*sizeof(uint32_t);
    unsigned int totalStructCount = GetAPICStructureCount();
	
	while(i < totalStructCount)
	{
	    APICStructureHeader* struc = (APICStructureHeader*)(start + curOffset);
	    if(struc == apicStruct)
            return i;
	    
		curOffset += struc->Length;
        i++;
	}
	
    return -1;
}

APICStructureHeader* MADT::GetNextAPICStructOfType(APICStructureHeader* previous, ApicStructureTypes type)
{
    //Sanity check for supplied pointer
    ASSERT(((Address)previous) >= GetAddress() && ((Address)previous) < GetAddress() + GetLengthInternal(), "Previous pointer doesn't point to a valid location in this MADT");
    
    if(!previous)
        return NULL;
    
    //Determine start point for search (i.e. next element)
    int i = GetAPICStructIndex(previous);
    if(i < 0)
        return NULL;
    
    //Determine start address for search
    Address curAddress = (Address)previous + previous->Length;
    unsigned int totalStructCount = GetAPICStructureCount();
    
    while((unsigned)i < totalStructCount)
    {
        APICStructureHeader* curStruct = (APICStructureHeader*)curAddress;
        if(curStruct->Type == type)
            return curStruct;
        
        curAddress += curStruct->Length; //next element
        i++;
    }
    
    return NULL;
}
