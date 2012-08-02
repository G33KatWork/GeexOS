#include <arch/x86Paging.h>
#include <string.h>
#include <arch/HAL.h>

using namespace Arch;
using namespace Debug;

void x86Paging::Init()
{
    //FIXME: use new loader information to setup paging
    ARCH_PAGING_DEBUG_MSG("Initializing Paging...");
    
    kernel_directory = new(true) x86PageDirectory();
    
    Address cr3;
    asm __volatile__ ( "mov %%cr3, %%eax" : "=a"(cr3) : );
    x86PageTable** tables = (x86PageTable**)cr3;
    
    //loop through all page directory entries and create them new inside kernel space
    //do not map the first 4 MB. This was loader memory and will be thrown away.
    for(int i = 1; i < 1024; i++)
    {
        if(tables[i])   //if there was a mapping...
        {
            ARCH_PAGING_DEBUG_MSG("Copying page table " << dec << i);
            
            //remove the status bits from the address (present, rw etc.)
            x86PageTable* table = (x86PageTable*)(((Address)tables[i]) & 0xFFFFF000);
            
            //allocate new table in kernel space
            x86PageTable* ourTable = new(true) x86PageTable();
            
            //copy the contents
            memcpy(ourTable, table, sizeof(x86PageTable));
            
            //determine the physical address of the newly allocated table
            //this makes several assumptions:
            //  - paging was activated by the loader
            //  - the memory where the page directory and corresponding page tables live
            //    is identity mapped
            Address physicalOfTable = earlyFindPhysicalAddress((Address)ourTable);
            
            //finally set the new table in the directory
            kernel_directory->SetTable(i, ourTable, physicalOfTable | 0x3);
        }
    }
    
    current_directory = kernel_directory;
    Address kernelPagedirPtr = earlyFindPhysicalAddress((Address)kernel_directory);
    
    ARCH_PAGING_DEBUG_MSG("Switching to PageDirectory with physical address " << hex << (unsigned)kernelPagedirPtr << "...");
    SwitchPageDirectory(kernelPagedirPtr);
}

void x86Paging::InitDone()
{
    /*ARCH_PAGING_DEBUG_MSG("HAL Initialization done, removing lowermost mapping");
    kernel_directory->SetTable(0, NULL, NULL);
    
    for(Address i = 0; i < 4*1024*1024; i+=PAGE_SIZE)
        asm volatile("invlpg %0"::"m" (*(char *)i));*/
        
    //Allocate remaining page tables in kernel land, for easy cloning of address spaces for userspace later
    //TODO: Find a better way to do this. This are 2MB of RAM! :-/
    ARCH_PAGING_DEBUG_MSG("Creating pagetables of uppermost gigabyte for kernel");
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
    
    x86PageTable *t = current_directory->GetTable(pdindex, true);
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
    
    x86PageTable *t = current_directory->GetTable(pdindex, true);
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
    
    x86PageTable *t = current_directory->GetTable(pdindex, false);
	ARCH_PAGING_DEBUG_MSG("Address of PageTable: " << (unsigned)t);
	if(t != NULL && (current_directory->GetTablePhysical(pdindex) & 0x1) == 1)
	{
    	x86Page *p = t->GetPage(ptindex);
    	ARCH_PAGING_DEBUG_MSG("Address of Page: " << (unsigned)p);
    
    	return p->Present();
	}
	else
		return false;
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
        pd->SetTable(pdindex, current_directory->GetTable(pdindex), current_directory->GetTablePhysical(pdindex) | 0x3);
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

Address x86Paging::earlyFindPhysicalAddress(Address virt)
{
    ARCH_PAGING_DEBUG_MSG("Early physical address from " << hex << virt << " requested.");
    
    unsigned int pdindex = virt >> 22;
    unsigned int ptindex = (virt >> 12) & 0x03FF;
    
    Address cr3;
    asm __volatile__ ( "mov %%cr3, %%eax" : "=a"(cr3) : );
    x86PageTable** tables = (x86PageTable**)cr3;
    
    x86PageTable* table = (x86PageTable*)(((Address)tables[pdindex]) & 0xFFFFF000);
    
    Address phys = table->GetPage(ptindex)->Frame() + (virt & 0xFFF);
    ARCH_PAGING_DEBUG_MSG("Early physical address is " << hex << phys);
    
    return phys;
}
