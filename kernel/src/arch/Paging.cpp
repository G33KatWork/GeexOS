#include <arch/Paging.h>
#include <lib/types.h>
#include <lib/string.h>
#include <kernel/global.h>
#include <kernel/debug.h>

using namespace Arch;
using namespace IO;

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
    ARCH_PAGING_DEBUG_MSG("Physical address of virtual " << hex << (unsigned)virtualaddr << " requested");
    
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
    ARCH_PAGING_DEBUG_MSG("Mappging physical " << hex << (unsigned)phys << " to virtual " << (unsigned)virt);
    ARCH_PAGING_DEBUG_MSG("Flags: " << (readwrite?"rw":"ro") << " " << (usermode?"umode":"kmode"));
    
    unsigned int pdindex = virt >> 22;
    unsigned int ptindex = (virt >> 12) & 0x03FF;
    
    PageTable *t = kernel_directory->GetTable(pdindex, true);
    ARCH_PAGING_DEBUG_MSG("Address of PageTable: " << (unsigned)t);
    Page *p = t->GetPage(ptindex);
    ARCH_PAGING_DEBUG_MSG("Address of Page: " << (unsigned)p);
    
    if(!p->Present()) //mapped? TODO: really mapped?        
        p->Present(true);

    p->Frame(phys);
    p->RW(readwrite);
    p->User(usermode);

    asm volatile("invlpg %0"::"m" (*(char *)virt));
}

void Paging::UnmapAddress(Address virt)
{
    ARCH_PAGING_DEBUG_MSG("Unmappging virtual " << hex << (unsigned)virt);
    
    unsigned int pdindex = virt >> 22;
    unsigned int ptindex = (virt >> 12) & 0x03FF;
    
    PageTable *t = kernel_directory->GetTable(pdindex, true);
    ARCH_PAGING_DEBUG_MSG("Address of PageTable: " << (unsigned)t);
    Page *p = t->GetPage(ptindex);
    ARCH_PAGING_DEBUG_MSG("Address of Page: " << (unsigned)p);
    
    p->Present(false);
    p->Frame(0);
    p->RW(false);
    
    asm volatile("invlpg %0"::"m" (*(char *)virt));
}

void Paging::SwitchCurrentPageDirectory(PageDirectory* dir)
{
    current_directory = dir;
    SwitchPageDirectory(GetPhysicalAddress((Address)(current_directory->tablesPhysical)));
}

PageTable* PageDirectory::GetTable(unsigned int index, bool assign)
{
    //create new page table if we should and there is no table yet
    if(assign && tables[index] == NULL)
    {
        ARCH_PAGING_DEBUG_MSG("PageTable with index " << index << " is not present and will be created.");
        
        PageTable* t = new (true /*page align*/) PageTable();
        Address physicalPageTableAddress = Paging::GetInstance()->GetPhysicalAddress((Address) t);
        ARCH_PAGING_DEBUG_MSG("Physical address of new PageTable: " << (unsigned)physicalPageTableAddress);
        SetTable(index, t, physicalPageTableAddress | 0x3);
    }
    
    return tables[index];
}
