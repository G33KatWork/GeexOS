#ifndef _ARCH_PAGING_H
#define _ARCH_PAGING_H

#include <lib/types.h>

#define IDENTITY_POSITION   0xFFFFF000
#define PAGE_SIZE           0x1000

namespace Arch
{
    class Paging;
    
    class Page
    {
        friend class Paging;
        friend class PageTable;
        
    private:
        Page() : present(0), rw(0), user(0), writethrough(0),
                 cachedisabled(0), accessed(0), dirty(0),
                 zero(0), global(0), unused(0), frame(0)
        {}
    
        bool Present()          { return present == 1; }
        bool RW()               { return rw == 1; }
        bool User()             { return user == 1; }
        bool WriteThrough()     { return writethrough == 1; }
        bool CacheDisabled()    { return cachedisabled == 1; }
        bool Accessed()         { return accessed == 1; }
        bool Dirty()            { return dirty == 1; }
        bool Global()           { return global == 1; }
        Address Frame()         { return frame << 12; }
    
        void Present(bool p)    { p ? present = 1 : present = 0; }
        void RW(bool r)         { r ? rw = 1 : rw = 0; }
        void User(bool u)       { u ? user = 1 : user = 0; }
        void WriteThrough(bool w)    { w ? writethrough = 1 : writethrough = 0; }
        void CacheDisabled(bool c)   { c ? cachedisabled = 1 : cachedisabled = 0; }
        void Accessed(bool a)   { a ? accessed = 1 : accessed = 0; }
        void Dirty(bool d)      { d ? dirty = 1 : dirty = 0; }
        void Global(bool g)     { g ? global = 1 : global = 0; }
        void Frame(Address a)   { frame = (a >> 12); }
        
        
        unsigned int present        : 1;   // Page present in memory
        unsigned int rw             : 1;   // Read-only if clear, readwrite if set
        unsigned int user           : 1;   // Supervisor level only if clear
        unsigned int writethrough   : 1;   // Write-Through caching or Write-Back
        unsigned int cachedisabled  : 1;   // Disable caching for this page
        unsigned int accessed       : 1;   // Has the page been accessed since last refresh?
        unsigned int dirty          : 1;   // Has the page been written to since last refresh?
        unsigned int zero           : 1;   // Must be zero
        unsigned int global         : 1;   // Don't update cache in TLB if CR3 is reset
        unsigned int unused         : 3;   // Amalgamation of unused bits
        unsigned int frame          : 20;  // Frame address (shifted right 12 bits)
    };

    class PageTable
    {
        friend class Paging;
        
    private:
        PageTable()
        {
            for(int i = 0; i < 1024; i++)
                pages[i] = Page();
        }
    
        Page *GetPage(unsigned int index)
        {
            return &(pages[index]);
        }
    
        void SetPage(unsigned int index, Page* page)
        {
            if(page == NULL)
                pages[index] = Page();
            else
                pages[index] = *page;
        }
    
    
        Page pages[1024];
    };

    class PageDirectory
    {
        friend class Paging;
        
    private:
        PageDirectory()
        {
            for (int i = 0; i < 1024; i++)
            {
                tablesPhysical[i] = NULL;
                tables[i] = NULL;
            }
        }
        
        Address GetTablePhysical(unsigned int index, bool assign = false)
        {
            //TODO: Handle assign
            return tablesPhysical[index];
        }
        
        PageTable* GetTable(unsigned int index, bool assign = false)
        {
            //TODO: Handle assign
            return tables[index];
        }
        
        void SetTable(unsigned int index, PageTable* table, Address tablePhysical)
        {
            tablesPhysical[index] = tablePhysical;
            tables[index] = table;
        }
        
        void SetIdentityTable(Address table)
        {
            tablesPhysical[1023] = table;
        }
        
        Address GetIdentityTable()
        {
            return tablesPhysical[1023];
        }
        
        
        Address tablesPhysical[1024];
        PageTable* tables[1024];
    };
    
    class Paging
    {
    public:
        static Paging* GetInstance();
        
        void Init(void);
        
        Address GetPhysicalAddress(Address virtualaddr);
        void MapAddress(Address virt, Address phys, bool readwrite, bool usermode);
        
        void SwitchCurrentPageDirectory(PageDirectory* dir);
        
        PageDirectory* GetKernelDirectory() { return kernel_directory; }
        
    private:
        static Paging* instance;
        
        Paging(){};
        
        PageDirectory *kernel_directory;
        PageDirectory *current_directory;
        
        PageTable *lowpagetable;
        
        inline void SwitchPageDirectory(Address pagedirectoryPhysical)
        {
            asm volatile (	"mov %0, %%eax\n"
    			"mov %%eax, %%cr3\n"
    			"mov %%cr0, %%eax\n"
    			"orl $0x80000000, %%eax\n"
    			"mov %%eax, %%cr0\n" :: "m" (pagedirectoryPhysical));
        }
    };
}

#endif
