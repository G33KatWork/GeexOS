#include <kernel/Memory/Heap.h>
#include <kernel/Memory/Paging.h>
#include <kernel/global.h>

using namespace Memory;
using namespace IO;

Heap::Heap(Address location, unsigned int size, unsigned int initialSize)
{
    __flp = NULL;
	begin = location;
    endAssigned = begin;
    __brkval = 0;
	this->maxSize = size;
	
	while(endAssigned - begin < initialSize)
        Expand();
        
    kout << "Heap created at: " << hex << (unsigned)location << " with max size of " << size << "B and initial size of" << initialSize << "B" << endl;
}

void Heap::Expand() {
    //FIXME: make it work
    kout << "Expanding..." << endl;
    
    Address newFrame = memoryManager.AllocateFrame();
    kout << "Got frame at phys: " << (unsigned)newFrame << " Mapping at virt " << (unsigned)endAssigned << endl;
    
    //for(int i = 0; i < 100000000; i++){}
    
    Paging::GetInstance()->MapAddress(endAssigned, newFrame, true, false);
    endAssigned += PAGE_SIZE; //FRAME SIZE
    
    //for(int i = 0; i < 100000000; i++){}
    kout << "Page allocated..." << endl;
    kout << "brkval: " << (unsigned)__brkval << endl << " endAssigned: " << (unsigned)endAssigned << endl << endl;
}

void* Heap::Allocate(size_t len, bool pageAlign)
{
    __freelist *fp1, *fp2;
	Address cp;
	size_t s, avail;

	/*
	 * Our minimum chunk size is the size of a pointer (plus the
	 * size of the "sz" field, but we don't need to account for
	 * this), otherwise we could not possibly fit a freelist entry
	 * into the chunk later.
	 */
	if (len < sizeof(struct __freelist) - sizeof(size_t))
		len = sizeof(struct __freelist) - sizeof(size_t);
	
    /*
	 * First, walk the free list and try finding a chunk that
	 * would match exactly.  If we found one, we are done.  While
	 * walking, note down the size of the largest chunk we found
	 * that would still fit the request -- we need it for step 2.
	 *
	 */
	for (s = 0, fp1 = __flp, fp2 = 0;
	     fp1;
	     fp2 = fp1, fp1 = fp1->nx) {
		if (fp1->sz == len) {
			/*
			 * Found it.  Disconnect the chunk from the
			 * freelist, and return it.
			 */
			if (fp2)
				fp2->nx = fp1->nx;
			else
				__flp = fp1->nx;
			return &(fp1->nx);
		}
		if (fp1->sz > len) {
			if (s == 0 || fp1->sz < s)
				s = fp1->sz;
		}
	}

	/*
	 * Step 2: If we found a chunk on the freelist that would fit
	 * (but was too large), look it up again and use it, since it
	 * is our closest match now.  Since the freelist entry needs
	 * to be split into two entries then, watch out that the
	 * difference between the requested size and the size of the
	 * chunk found is large enough for another freelist entry; if
	 * not, just enlarge the request size to what we have found,
	 * and use the entire chunk.
	 */
	if (s) {
		if (s - len < sizeof(struct __freelist))
			len = s;
		for (fp1 = __flp, fp2 = 0;
		     fp1;
		     fp2 = fp1, fp1 = fp1->nx) {
			if (fp1->sz == s) {
				if (len == s) {
					/*
					 * Use entire chunk; same as
					 * above.
					 */
					if (fp2)
						fp2->nx = fp1->nx;
					else
						__flp = fp1->nx;
					return &(fp1->nx);
				}
				/*
				 * Split them up.  Note that we leave
				 * the first part as the new (smaller)
				 * freelist entry, and return the
				 * upper portion to the caller.  This
				 * saves us the work to fix up the
				 * freelist chain; we just need to
				 * fixup the size of the current
				 * entry, and note down the size of
				 * the new chunk before returning it
				 * to the caller.
				 */
				cp = (Address)fp1;
				s -= len;
				cp += s;
				fp2 = (struct __freelist *)cp;
				fp2->sz = len;
				fp1->sz = s - sizeof(size_t);
				return &(fp2->nx);
			}
		}
	}

	/*
	 * Step 3: If the request could not be satisfied from a
	 * freelist entry, just prepare a new chunk.  This means we
	 * need to obtain more memory first.  The largest address just
	 * not allocated so far is remembered in the brkval variable.
	 * Under Unix, the "break value" was the end of the data
	 * segment as dynamically requested from the operating system.
	 * Since we don't have an operating system, just make sure
	 * that we don't collide with the stack.
	 */
	if (__brkval == 0)
		__brkval = begin;
	cp = begin + maxSize;
#if 0
	if (cp == 0)
		cp = STACK_POINTER() - __malloc_margin;
#endif
	avail = cp - __brkval;
	
    /*kout << "brkval: " << (unsigned)__brkval << endl << " cp: " << (unsigned)cp << endl << " avail: " << (unsigned)avail << endl;
    for(;;){}*/
    
    //do we need to map new frame/frames into the heap?
    while(__brkval + len >= endAssigned)
    {
        kout << "__brkval: " << hex << (unsigned)__brkval << endl;
        kout << "len: " << hex << (unsigned)len << endl;
        kout << "endAssigned: " << hex << (unsigned)endAssigned << endl;
        Expand();
    }

	/*
	 * Both tests below are needed to catch the case len >= 0xfffe.
	 */
	if (avail >= len && avail >= len + sizeof(size_t)) {
		fp1 = (struct __freelist *)__brkval;
		__brkval += len + sizeof(size_t);
		fp1->sz = len;
		return &(fp1->nx);
	}

	/*
	 * Step 4: There's no help, just fail. :-/
	 */
	return 0;
}

void Heap::Deallocate(void* p)
{
    
}
