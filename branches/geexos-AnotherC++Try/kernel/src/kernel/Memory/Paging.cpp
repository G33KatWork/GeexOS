#include <kernel/Memory/Paging.h>
#include <lib/types.h>
#include <kernel/global.h>

using namespace Memory;
using namespace Arch;

using namespace IO;

void Paging::Init()
{
    Address kernelpagedirPtr = 0;
    Address lowpagetablePtr = 0;
    int k = 0;
    
    kernel_directory = new (true) PageDirectory();
    lowpagetable = new (true) PageTable();
    
    //kout << hex << (unsigned)kernel_directory << endl;
    
    kernelpagedirPtr = (Address)kernel_directory + 0x40000000;
    lowpagetablePtr = (Address)lowpagetable + 0x40000000;
    
    for (k = 0; k < 1024; k++)
	{
        lowpagetable->GetPage(k)->Frame(k * 4096);
        lowpagetable->GetPage(k)->Present(true);
        lowpagetable->GetPage(k)->RW(true);
	}
	
	for(k = 0; k < 1023; k++)
		kernel_directory->SetTable(k, NULL);
	
    kernel_directory->SetIdentityTable((PageTable *)(kernelpagedirPtr | 0x3));
    
    kernel_directory->SetTable(0, (PageTable *)(lowpagetablePtr | 0x3));
	kernel_directory->SetTable(768, (PageTable *)(lowpagetablePtr | 0x3));
	
    SwitchPageDirectory(kernelpagedirPtr);
}
