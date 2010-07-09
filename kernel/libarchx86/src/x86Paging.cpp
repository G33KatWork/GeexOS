#include <arch/x86Paging.h>
#include <string.h>
#include <kernel/global.h>
#include <kernel/debug.h>
#include <arch/HAL.h>

using namespace Arch;
using namespace Debug;

void x86Paging::Init()
{
    ARCH_PAGING_DEBUG_MSG("Initializing Paging...");
    
    Address kernelpagedirPtr = 0;
    Address lowpagetablePtr = 0;
    int k = 0;
    
    kernel_directory = new(true /*page allocation*/) x86PageDirectory();
    ARCH_PAGING_DEBUG_MSG("Kernel PageDirectory allocated at " << hex << (unsigned)kernel_directory);
    lowpagetable = new(true /*page allocation*/) x86PageTable();
    ARCH_PAGING_DEBUG_MSG("First PageTable for lowermost 4MB allocated at " << hex << (unsigned)lowpagetable);
    
    kernelpagedirPtr = (Address)kernel_directory + 0x40000000;
    lowpagetablePtr = (Address)lowpagetable + 0x40000000;
    
    for (k = 0; k < 1024; k++)
	{
        lowpagetable->GetPage(k)->Frame(k * PAGE_SIZE);
        lowpagetable->GetPage(k)->Present(true);
        lowpagetable->GetPage(k)->RW(true);
	}
	
    //kernel_directory->SetIdentityTable((Address)(kernelpagedirPtr | 0x3));
    
    kernel_directory->SetTable(0, lowpagetable, (Address)(lowpagetablePtr | 0x3));
	kernel_directory->SetTable(768, lowpagetable, (Address)(lowpagetablePtr | 0x3));
	
    current_directory = kernel_directory;
	
	ARCH_PAGING_DEBUG_MSG("Switching to PageDirectory with physical address " << hex << (unsigned)kernelpagedirPtr << "...");
    SwitchPageDirectory(kernelpagedirPtr);
    ARCH_PAGING_DEBUG_MSG("Switching successful");
}

void x86Paging::InitDone()
{
    kernel_directory->SetTable(0, NULL, NULL);
    
    for(Address i = 0; i < 4*1024*1024; i+=PAGE_SIZE)
        asm volatile("invlpg %0"::"m" (*(char *)i));
        
    //Allocate remaining page tables in kernel land, for easy cloning of address spaces for userspace later
    //TODO: Find a better way to do this. This are 2MB of RAM! :-/
    for(int i = 769; i < 1024; i++)
        kernel_directory->GetTable(i, true);
}

Address x86Paging::GetPhysicalAddress(Address virtualaddr)
{
    ARCH_PAGING_DEBUG_MSG("Physical address of virtual " << hex << virtualaddr << " requested");
    
    unsigned int pdindex = virtualaddr >> 22;
    unsigned int ptindex = (virtualaddr >> 12) & 0x03FF;
    Address offset = virtualaddr & 0xFFF;

    x86PageTable* pt = current_directory->GetTable(pdindex, false);
    if(pt == NULL)
        return NULL;
    
    x86Page* p = pt->GetPage(ptindex);
    if(!p->Present())
        return NULL;
    
    return p->Frame() + offset;
    
    //unsigned int * pt2 = ((unsigned int *)0xFFC00000) + (0x400 * pdindex);
    //return ((pt2[ptindex] & ~0xFFF) + ((unsigned int)virtualaddr & 0xFFF));
}

void x86Paging::MapAddress(Address virt, Address phys, bool readwrite, bool usermode)
{   
    ARCH_PAGING_DEBUG_MSG("Mappging physical " << hex << phys << " to virtual " << virt);
    ARCH_PAGING_DEBUG_MSG("Flags: " << (readwrite?"rw":"ro") << " " << (usermode?"umode":"kmode"));
    
    unsigned int pdindex = virt >> 22;
    unsigned int ptindex = (virt >> 12) & 0x03FF;
    
    x86PageTable *t = kernel_directory->GetTable(pdindex, true);
    ARCH_PAGING_DEBUG_MSG("Address of PageTable: " << (unsigned)t);
    x86Page *p = (x86Page*)t->GetPage(ptindex);
    ARCH_PAGING_DEBUG_MSG("Address of Page: " << (unsigned)p);
    
    if(!p->Present()) //mapped? TODO: really mapped?        
        p->Present(true);

    p->Frame(phys);
    p->RW(readwrite);
    p->User(usermode);

    asm volatile("invlpg %0"::"m" (*(char *)virt));
}

void x86Paging::UnmapAddress(Address virt)
{
    ARCH_PAGING_DEBUG_MSG("Unmappging virtual " << hex << virt);
    
    unsigned int pdindex = virt >> 22;
    unsigned int ptindex = (virt >> 12) & 0x03FF;
    
    x86PageTable *t = kernel_directory->GetTable(pdindex, true);
    ARCH_PAGING_DEBUG_MSG("Address of PageTable: " << (unsigned)t);
    x86Page *p = t->GetPage(ptindex);
    ARCH_PAGING_DEBUG_MSG("Address of Page: " << (unsigned)p);
    
    p->Present(false);
    p->Frame(0);
    p->RW(false);
    
    asm volatile("invlpg %0"::"m" (*(char *)virt));
}    

bool x86Paging::IsPresent(Address virt)
{
    ARCH_PAGING_DEBUG_MSG("Checking if virtual " << hex << virt << " is mapped");
    
    unsigned int pdindex = virt >> 22;
    unsigned int ptindex = (virt >> 12) & 0x03FF;
    
    x86PageTable *t = (x86PageTable*)kernel_directory->GetTable(pdindex, true);
    ARCH_PAGING_DEBUG_MSG("Address of PageTable: " << (unsigned)t);
    x86Page *p = (x86Page*)t->GetPage(ptindex);
    ARCH_PAGING_DEBUG_MSG("Address of Page: " << (unsigned)p);
    
    return p->Present();
}

void x86Paging::SwitchCurrentPageDirectory(BasePageDirectory* dir)
{
    current_directory = (x86PageDirectory*)dir;
    SwitchPageDirectory(GetPhysicalAddress((Address)(current_directory->tablesPhysical)));
}

BasePageDirectory* x86Paging::DeriveUserspaceDirectory()
{
    x86PageDirectory* pd = new(true /*page allocation*/) x86PageDirectory();
    
    for(Address i = 0xC0000000; i < 0xFFFFFFFF; i+= 4*1024*1024)
    {
        unsigned int pdindex = i / 4*1024*1024;
        pd->SetTable(pdindex, kernel_directory->GetTable(pdindex), kernel_directory->GetTablePhysical(pdindex) | 0x3);
    }
    
    return pd;
}

x86PageTable* x86PageDirectory::GetTable(unsigned int index, bool assign)
{
    //create new page table if we should and there is no table yet
    if(assign && tables[index] == NULL)
    {
        ARCH_PAGING_DEBUG_MSG("PageTable with index " << index << " is not present and will be created.");
        
        x86PageTable* t = new (true /*page align*/) x86PageTable();
        Address physicalPageTableAddress = CurrentHAL->GetPaging()->GetPhysicalAddress((Address) t);
        ARCH_PAGING_DEBUG_MSG("Physical address of new PageTable: " << physicalPageTableAddress);
        SetTable(index, t, physicalPageTableAddress | 0x3);
    }
    
    return tables[index];
}
