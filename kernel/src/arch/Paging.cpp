#include <arch/Paging.h>
#include <lib/types.h>
#include <lib/string.h>
#include <kernel/global.h>

using namespace Memory;
using namespace Arch;

Paging* Paging::instance = NULL;

Paging* Paging::GetInstance()
{
    if(instance == NULL)
        instance = new Paging();

    return instance;
}

void Paging::Init()
{
    Address kernelpagedirPtr = 0;
    Address lowpagetablePtr = 0;
    int k = 0;
    
    kernel_directory = new(true /*page allocation*/) PageDirectory();
    lowpagetable = new(true /*page allocation*/) PageTable();
    
    kernelpagedirPtr = (Address)kernel_directory;
    lowpagetablePtr = (Address)lowpagetable;
    
    for (k = 0; k < 1024; k++)
	{
        lowpagetable->GetPage(k)->Frame(k * PAGE_SIZE);
        lowpagetable->GetPage(k)->Present(true);
        lowpagetable->GetPage(k)->RW(true);
	}
	
    kernel_directory->SetIdentityTable((Address)(kernelpagedirPtr | 0x3));
    
    //Map first physical 4 MB at virtual 0x0
    kernel_directory->SetTable(0, lowpagetable, (Address)(lowpagetablePtr | 0x3));
	
    current_directory = kernel_directory;
	
    SwitchPageDirectory(kernelpagedirPtr);
}

Address Paging::GetPhysicalAddress(Address virtualaddr)
{
    unsigned int pdindex = virtualaddr >> 22;
    unsigned int ptindex = (virtualaddr >> 12) & 0x03FF;

    unsigned int *pd = (unsigned int*)0xFFFFF000;
    ASSERT(*pd & 0x01, "Page for identity paging is not present");

    unsigned int * pt = ((unsigned int *)0xFFC00000) + (0x400 * pdindex);
    ASSERT(*pt & 0x01, "Pagetable for identity paging is not present");

    return ((pt[ptindex] & ~0xFFF) + ((unsigned int)virtualaddr & 0xFFF));
}

void Paging::MapAddress(Address virt, Address phys, bool readwrite, bool usermode)
{   
    unsigned int pdindex = virt >> 22;
    unsigned int ptindex = (virt >> 12) & 0x03FF;
    
    PageTable *t = kernel_directory->GetTable(pdindex, true);
    
    Page *p = t->GetPage(ptindex);
    if(!p->Present()) //mapped? TODO: really mapped?
    {
        p->Frame(phys);
        p->Present(true);
        p->RW(readwrite);
        p->User(usermode);
    }
}

void Paging::SwitchCurrentPageDirectory(PageDirectory* dir)
{
    current_directory = dir;
    SwitchPageDirectory(GetPhysicalAddress((Address)(current_directory->tablesPhysical)));
}

using namespace IO;
PageTable* PageDirectory::GetTable(unsigned int index, bool assign)
{
    //create new page table if we should and there is no table yet
    if(assign && tables[index] == NULL)
    {
        PageTable* t = new (true /*page align*/) PageTable();
        Address physicalPageTableAddress = Paging::GetInstance()->GetPhysicalAddress((Address) t);
        SetTable(index, t, physicalPageTableAddress | 0x3);
    }
    
    return tables[index];
}
