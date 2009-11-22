#include <kernel/Memory/Heap.h>
#include <arch/Paging.h>
#include <kernel/global.h>

using namespace Memory;
using namespace Arch;
using namespace IO;

//#define DEBUG_HEAP_MSG(x) DEBUG_MSG("HEAP: " << x)
#define DEBUG_HEAP_MSG(x) ;

void Heap::DumpCurrentStructure(CharacterOutputDevice& out)
{
    const char* header = "digraph g {\n" \
    "graph ["
    "rankdir = \"LR\"\n" \
    "];\n" \
    "\n" \
    "node [\n" \
    "fontsize = \"16\"\n" \
    "shape = \"ellipse\"\n" \
    "];\n" \
    "\n" \
    "edge [\n" \
    "];\n" \
    "\n" \
    "\n" \
    "\"legend\" [\n" \
    "label = \"<f0>Adress of struct| <f1> Size of Block| <f2> is free?| <f3> Address of next struct\"\n" \
    "shape = \"record\"\n" \
    "];\n" \
    "\n" \
    "\"start\" [\n" \
    "label = \"<f0> startAddress\"\n" \
    "shape = \"record\"\n" \
    "];\n";
    const char* footer = "\n}\n";
    
    out << header;
    
    int nodeCounter = 1;
    list_item *curItem, *lastItem = NULL;
    for(curItem = (list_item*)startAddress; curItem != NULL; curItem = curItem->next)
    {
        out << "\"node" << dec << nodeCounter << "\" [\n";
        out << "label = \"<f0> " << hex << (unsigned)curItem << "| <f1>" << (unsigned)curItem->size << "| <f2>" << (curItem->free ? "true" : "false") << "| <f3>" << hex << (unsigned)curItem->next << "\"\n";
        out << "shape = \"record\"\n";
        out << "];\n";
        
        if(lastItem != NULL)
        {
            out << "\"node" << dec << (nodeCounter-1) << "\":f3 -> \"node" << nodeCounter << "\":f0 [\n";
            out << "id = " << nodeCounter << "\n";
            out << "];\n";
        }
        
        nodeCounter++;
        lastItem = curItem;
    }
    
    out << "\"start\":f0 -> \"node1\":f0 [\n";
    out << "id = 1";
    out << "];\n";
    
    out << footer;
}

void Heap::expand(size_t newSize)
{
    DEBUG_HEAP_MSG("Heap expanding to " << hex << (unsigned)newSize);
    ASSERT(newSize > getCurrentSize(), "Cannot shrink a heap in an expand function");
    
    //get nearest following page boundary
    if(newSize % PAGE_SIZE != 0)
    {
        newSize &= IDENTITY_POSITION;
        newSize += PAGE_SIZE;
    }
    
    ASSERT(startAddress + newSize <= maxAddress, "Heap got too big... Can't expand anymore.");
    
    size_t oldSize = getCurrentSize();
    
    while(oldSize < newSize)
    {   
        Address newFrame = memoryManager.AllocateFrame();
        Paging::GetInstance()->MapAddress(startAddress + oldSize, newFrame, true, false);
        
        DEBUG_HEAP_MSG("Mapped frame " << hex << (unsigned)newFrame << " to: " << hex << (unsigned)(startAddress + oldSize));
        
        oldSize += PAGE_SIZE;
    }
    
    endAddress = startAddress + newSize;
}    

void Heap::expandTo(Address upperAddress)
{
    size_t currentSize = getCurrentSize();
    size_t additionalSpace = upperAddress - this->endAddress;
    
    this->expand(currentSize + additionalSpace);
}

list_item* Heap::findFirstFit(size_t size, bool pageAlign)
{
    list_item* found = NULL;
    for(list_item* curItem = (list_item*)startAddress; found == NULL && curItem != NULL; curItem = curItem->next)
    {
        //DEBUG_HEAP_MSG("curItem = " << hex << (unsigned)curItem);
        if(curItem->size >= size && curItem->free)
        {
            found = curItem;
            
            if(pageAlign)
            {
                Address location = (Address)curItem;
                int offset = 0;
                
                //calculate offset we need to apply to get this stuff page aligned
                if ((((location+sizeof(list_item))) & IDENTITY_POSITION) != 0)
    				offset = PAGE_SIZE - (location+sizeof(list_item)) % PAGE_SIZE;
    				
                size_t alignedSize = curItem->size - offset;
                
                //do we still fit?
                if(alignedSize < size)
                    found = NULL;
            }
        }
    }
    
    return found;
}

Heap::Heap(Address location, size_t size, size_t initialSize)
{
    DEBUG_HEAP_MSG("Size of list_item is " << dec << (unsigned)sizeof(list_item));
    
    //Check for heap location to be page aligned and that the supplied parameters are fine
    ASSERT(location % PAGE_SIZE == 0, "Heap is not page aligned");
    ASSERT((location + size) % PAGE_SIZE == 0, "Heap is not page aligned");
    //ASSERT(initialSize > HEAP_INDEX_SIZE * sizeof(void*), "Initial size must be big enough to contain all indices");
    
    //Set some basic data
    this->startAddress = location;
    this->endAddress = startAddress;
    this->maxAddress = location + size;
    
    //Acquire some initial space from the paging subsystem
    this->expand(initialSize);
    
    //Setup the first hole
    list_item *firstItem = (list_item*)this->startAddress;
    firstItem->size = size - (sizeof(list_item));
    firstItem->free = true;
    firstItem->next = NULL;
}

void* Heap::Allocate(size_t len, bool pageAlign)
{
    DEBUG_HEAP_MSG("Request for " << dec << (unsigned)len << " Bytes - Page Aligned? " << (pageAlign ? "Yes" : "No"));
    
    list_item* found = findFirstFit(len, pageAlign);
    DEBUG_HEAP_MSG("first block that fits found at " << hex << (unsigned)found);
    DEBUG_HEAP_MSG("found block has a size of " << dec << (unsigned)found->size);
    
    if(found == NULL)
        return NULL;
    
    //calculate offset to be applied to be page aligned
    Address location = (Address)found;
    int offset = 0;
    if ( pageAlign && ((((location+sizeof(list_item))) & IDENTITY_POSITION) != 0) )
		offset = PAGE_SIZE - (location+sizeof(list_item)) % PAGE_SIZE;
    
    //is it worth splitting this block?
    if(found->size - offset - len > sizeof(list_item))
    {   //yes
        DEBUG_HEAP_MSG("Splitting found block...");
        
        //save some old stuff
        size_t oldSize = found->size;
        list_item* oldNext = found->next;
        
        DEBUG_HEAP_MSG("Old size was " << dec << (unsigned)oldSize);
        
        //determine position of new element
        list_item* reminder = (list_item*)((Address)found + sizeof(list_item) + offset + len);
        
        DEBUG_HEAP_MSG("New block will be at " << hex << (unsigned)reminder);
        
        //check wether we need to expand the heap
        if(((Address)reminder) + sizeof(list_item) > this->endAddress)
            this->expandTo(((Address)reminder) + sizeof(list_item));
        
        //update the found block
        found->size = offset + len;
        
        found->next = reminder;
        
        //fill new element with appropriate data
        reminder->next = oldNext;
        reminder->size = oldSize - offset - len - sizeof(list_item);
        reminder->free = true;
        DEBUG_HEAP_MSG("And has a size of " << dec << (unsigned)reminder->size << " Bytes");
    }
    else    //no
    {
        DEBUG_HEAP_MSG("Reminder of found block is not worth splitting...");
        
        //fill requested length up to next block
        len = found->size;
    }
    
    found->free = false;
    
    DEBUG_HEAP_MSG("Found suitable block at " << hex << (unsigned)found);
    DEBUG_HEAP_MSG("Offset " << hex << (unsigned)offset << " will be applied");
    DEBUG_HEAP_MSG("Size of list_item is " << hex << (unsigned)sizeof(list_item));
    DEBUG_HEAP_MSG("Resulting return value is " << hex << (unsigned)(((unsigned)found) + sizeof(list_item) + offset));
    DEBUG_HEAP_MSG("Current heap size is " << hex << (unsigned)getCurrentSize());
    
    //check wether we need to expand the heap
    if(((Address)found) + sizeof(list_item) + offset + len > this->endAddress)
        this->expandTo(((Address)found) + sizeof(list_item) + offset + len);
    
    return (void*)((((Address)found)  /*- this->endAddress*/) + sizeof(list_item) + offset);
}

void Heap::Deallocate(void* p)
{
    //free our block
    list_item* block = (list_item*)(((Address)p) - sizeof(list_item));
    block->free = true;
    
    //TODO: contract two adjacent blocks if free
}
