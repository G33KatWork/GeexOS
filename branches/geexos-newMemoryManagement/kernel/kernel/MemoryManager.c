#include "MemoryManager.h"
#include "FrameAllocator.h"
#include "arch/x86/paging.h"

#include "drivers/serial.h"

// end of kernel defined in linker script
extern uint32_t end;
uint32_t placement_address = (uint32_t)&end;

static bool kheap_initialized = false;

/**
 * Start of our kernel heap
 * This is also the start of our linked list for the blocks
**/
static void* kheap_start = (void*)KHEAP_START;

/**
 * Before we initialize the kernel's heap our current end doesn't
 * yet exist
**/
static void* kheap_current_end = (void*)KHEAP_START;


struct mem_info {
    bool free;
    uint32_t size;
    struct mem_info *next_block;
} __attribute__((packed));


/**
 * Static method for memory allocation
 * Gets called by kmalloc() and kmalloc_a()
**/
static void* kmalloc_int(size_t size, bool page_aligned);

/**
 * Static method for expanding the heap by one page
**/
static void expandKheap(void);


void init_kheap(void)
{
    //get a first frame and map it into the address space
    uint32_t* allocated_frame = allocate_frame();
    paging_map_address(allocated_frame, kheap_current_end, 0x3); //Present and Read/Write
    
    //write a first metadata information
    struct mem_info *metadata = (struct mem_info *)kheap_current_end;
    metadata->free = true;
    metadata->size = KHEAP_SIZE - sizeof(struct mem_info);
    metadata->next_block = NULL;
    
    //Add one page to the pointer
    kheap_current_end += PAGE_SIZE;
    
    kheap_initialized = true;
}

static void expandKheap(void)
{
    printf_serial(COM1, "expanding heap...\n");
    
    uint32_t* allocated_frame = allocate_frame();
    paging_map_address(allocated_frame, kheap_current_end, 0x3); //Present and Read/Write
    kheap_current_end += PAGE_SIZE;
    
    printf_serial(COM1, "allocated frame: %x - new heap end: %x\n", allocated_frame, kheap_current_end);
}

static void* kmalloc_int(size_t size, bool page_aligned)
{
    uint32_t tmp;
    
    if(!kheap_initialized)
	{
	    if(page_aligned && (placement_address & 0xFFFFF000))
	    {
            // Align the placement address;
            placement_address &= 0xFFFFF000;
            placement_address += 0x1000;
	    }
	    
	    tmp = placement_address;
    	placement_address += size;
	}
	else
    {   //here we initialized our kernel heap
        //we will use a first fit algorithm to grab memory
        printf_serial(COM1, "asked for %u bytes\n", size);
        
        struct mem_info* nextBlock = (struct mem_info *)kheap_start;
        while(nextBlock != NULL)
        {
            //printf_serial(COM1, "looking for block. current block at %x\n", nextBlock);
            if(nextBlock->free && nextBlock->size >= size) //found something!
            {
                
                    printf_serial(COM1, "found empty block with suitables size at %x\n", nextBlock);
                    
                uint32_t dataAddress = ((uint32_t)nextBlock) + sizeof(struct mem_info);
                printf_serial(COM1, "address for saveable data is at %x\n", dataAddress);
                
                //check if we are in our heap bounds
                if(dataAddress + size > KHEAP_END)
                {
                    nextBlock = NULL;
                    continue;
                }
                
                while(dataAddress + size > (uint32_t)kheap_current_end)      //does the requested memory exceed the current size of the heap?
                    expandKheap();                                              //let it grow...
                
                //save old parameters    
                uint32_t oldSize = nextBlock->size;
                struct mem_info* followingBlock = nextBlock->next_block;
                
                //write new parameters
                nextBlock->size = size;
                nextBlock->free = false;
                
                //check if we have memory left
                if(oldSize > size)
                {
                    printf_serial(COM1, "we have much memory left, old size: %x\n", oldSize);
                    struct mem_info *newBlock = (struct mem_info *)(dataAddress + size);
                    
                    //do we need to expand the heap for writing the new "empty-block"?
                    printf_serial(COM1, "new block will be saved at: %x\n", newBlock);
                    printf_serial(COM1, "end of new block at: %x - current heap end at: %x\n", (uint32_t)newBlock + sizeof(struct mem_info), kheap_current_end);
                    
                    if((uint32_t)newBlock + sizeof(struct mem_info) > (uint32_t)kheap_current_end)
                        expandKheap();
                    
                    newBlock->free = true;
                    newBlock->size = oldSize - sizeof(struct mem_info) - size;      //old size - actual new header caused by split up - size of requested data
                    newBlock->next_block = followingBlock;
                    
                    //set new following block
                    nextBlock->next_block = newBlock;
                }
                
                printf_serial(COM1, "returning address: %x\n", dataAddress);
                return (void*)dataAddress;
            }
            else
                nextBlock = nextBlock->next_block;  //just go to the next block
        }
        
        // we didn't find anything suitable for our needs... BAD!
        printf_serial(COM1, "returning NULL\n");
        return NULL;
    }
    
	return (void*)tmp;
}


void* kmalloc(size_t size)
{
    return kmalloc_int(size, false);
}


void* kmalloc_a(size_t size)
{
    return kmalloc_int(size, true);
}

//TODO: Add free()
