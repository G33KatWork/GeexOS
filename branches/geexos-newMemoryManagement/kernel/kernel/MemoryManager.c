#include "MemoryManager.h"
#include "FrameAllocator.h"
#include "arch/x86/paging.h"

#include "drivers/serial.h"

// end of kernel defined in linker script
extern uint32_t end;
uint32_t placement_address = (uint32_t)&end;

/**
 * The current end of the heap
**/
static uint32_t kheap_current_end = KHEAP_START;



typedef long Align;    /* for alignment to long boundary */ 
union header {         /* block header */ 
    struct { 
        union header *ptr; /* next block if on free list */ 
        unsigned size;     /* size of this block */ 
    } s; 
    Align x;           /* force alignment of blocks */ 
};
typedef union header Header;


static Header base;       /* empty list to get started */
static Header *freep = NULL;     /* start of free list */ 


/**
 * Static method for memory allocation
 * Gets called by kmalloc() and kmalloc_a()
**/
static void* kmalloc_int(size_t size, bool page_aligned);

/**
 * Internal malloc with a more sophisticated algorithm used
**/
static void* kmalloc_list(size_t size);

/**
 * Adds some more bytes to te heap pool
**/
static Header *morecore(unsigned nu);

/**
 * Adds some more space to the kernel heap
**/
static void* sbrk(size_t d);


void init_kheap(void)
{
    //Initialize our list    
    base.s.ptr = freep = &base;
    base.s.size = 0;
}

static void* kmalloc_int(size_t size, bool page_aligned)
{
    uint32_t tmp;
    
    if(freep == NULL)
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
    {   
        return kmalloc_list(size);
    }
    
	return (void*)tmp;
}


static void* kmalloc_list(size_t size)
{
    Header *p, *prevp; 
    unsigned nunits;
    
    prevp = freep;
    
    nunits = (size+sizeof(Header)-1)/sizeof(Header) + 1;
           
    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) { 
        if (p->s.size >= nunits) {  /* big enough */ 
            if (p->s.size == nunits)  /* exactly */ 
                prevp->s.ptr = p->s.ptr; 
            else {              /* allocate tail end */ 
                p->s.size -= nunits; 
                p += p->s.size; 
                p->s.size = nunits; 
            } 
                freep = prevp; 
                return (void *)(p+1); 
        }
         
        if (p == freep)  /* wrapped around free list */ 
            if ((p = morecore(nunits)) == NULL) 
                return NULL;    /* none left */ 
    } 
}

#define NALLOC  1024   /* minimum #units to request */ 
static Header *morecore(unsigned nu) 
{ 
    void *cp; 
    Header *up; 
 
    if (nu < NALLOC) 
        nu = NALLOC; 
        
    cp = sbrk(nu * sizeof(Header)); 
    if (cp == (void *) -1)   /* no space at all */ 
        return NULL; 
    
    up = (Header *) cp; 
    up->s.size = nu; 
    free((void *)(up+1)); 
    return freep; 
}

static void* sbrk(size_t d)
{
    //Check bounds
    if(kheap_current_end + d > KHEAP_END)
        return (void*)-1;
    
    uint32_t times = d / PAGE_SIZE;
    void* retAddr = (void*)kheap_current_end;
    
    for(uint32_t i = 0; i < times; i++)
    {
        uint32_t* frame = allocate_frame();
        paging_map_address(frame, (uint32_t*)kheap_current_end, 0x3); //Present, Read/Write
        kheap_current_end += PAGE_SIZE;
    }
    
    return (void*)retAddr;
}


void free(void *ap) 
{ 
    Header *bp, *p; 
 
    bp = (Header *)ap - 1;    /* point to  block header */ 
    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr) 
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr)) 
            break;  /* freed block at start or end of arena */ 
 
    if (bp + bp->s.size == p->s.ptr) {    /* join to upper nbr */ 
        bp->s.size += p->s.ptr->s.size; 
        bp->s.ptr = p->s.ptr->s.ptr; 
    } else 
        bp->s.ptr = p->s.ptr; 
    
    if (p + p->s.size == bp) {            /* join to lower nbr */ 
        p->s.size += bp->s.size; 
        p->s.ptr = bp->s.ptr; 
    } else 
        p->s.ptr = bp; 
    
    freep = p; 
}


void* kmalloc(size_t size)
{
    return kmalloc_int(size, false);
}


void* kmalloc_a(size_t size)
{
    return kmalloc_int(size, true);
}
