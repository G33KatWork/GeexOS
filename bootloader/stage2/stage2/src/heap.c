/* Copyright (c) 2004, 2010 Joerg Wunsch
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/

/* Parts taken from avr-libc and modified for my needs. See License header
   above this notice for detailed licensing information. The copyright of
   Joerg Wunsch still applies
*/

#include <heap.h>
#include <memory.h>
#include <print.h>

Heap* heap_create(size_t MaxSize)
{
	Heap* heap;

	MaxSize = PAGE_END(MaxSize);
	heap = (Heap*)memory_allocate(MaxSize, MemoryTypeLoaderHeap);

	heap->MaxSize = MaxSize;
	heap->Freelist = NULL;
	heap->allocationCount = 0;
	heap->deallocationCount = 0;

	size_t dataSpace = MaxSize - sizeof(Heap) - sizeof(size_t);

	struct __freelist* firstBlock = (struct __freelist*)((uintptr_t)heap + sizeof(Heap));
	firstBlock->sz = dataSpace;
	firstBlock->nx = NULL;
	heap->Freelist = firstBlock;

	return heap;
}

void heap_destroy(Heap* heap)
{
	uint64_t pageCount = heap->MaxSize / arch_pagesize;
	uint64_t heapStart = (uintptr_t)heap / arch_pagesize;
	memory_mark_pages(heapStart, pageCount, MemoryTypeFree);
}

void* heap_alloc(Heap* heap, size_t len)
{
	struct __freelist *fp1, *fp2, *sfp1, *sfp2;
	char *cp;
	size_t s;

	if (len < sizeof(struct __freelist) - sizeof(size_t))
		len = sizeof(struct __freelist) - sizeof(size_t);

	heap->allocationCount++;

	/*
	 * First, walk the free list and try finding a chunk that
	 * would match exactly.  If we found one, we are done.  While
	 * walking, note down the smallest chunk we found that would
	 * still fit the request -- we need it for step 2.
	 *
	 */
	for (s = 0, fp1 = heap->Freelist, fp2 = 0;
	     fp1;
	     fp2 = fp1, fp1 = fp1->nx) {
		if (fp1->sz < len)
			continue;
		if (fp1->sz == len) {
			/*
			 * Found it.  Disconnect the chunk from the
			 * freelist, and return it.
			 */
			if (fp2)
				fp2->nx = fp1->nx;
			else
				heap->Freelist = fp1->nx;
			return &(fp1->nx);
		}
		else {
			if (s == 0 || fp1->sz < s) {
				/* this is the smallest chunk found so far */
				s = fp1->sz;
				sfp1 = fp1;
				sfp2 = fp2;
			}
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
		if (s - len < sizeof(struct __freelist)) {
			/* Disconnect it from freelist and return it. */
			if (sfp2)
				sfp2->nx = sfp1->nx;
			else
				heap->Freelist = sfp1->nx;
			return &(sfp1->nx);
		}
		/*
		 * Split them up.  Note that we leave the first part
		 * as the new (smaller) freelist entry, and return the
		 * upper portion to the caller.  This saves us the
		 * work to fix up the freelist chain; we just need to
		 * fixup the size of the current entry, and note down
		 * the size of the new chunk before returning it to
		 * the caller.
		 */
		cp = (char *)sfp1;
		s -= len;
		cp += s;
		sfp2 = (struct __freelist *)cp;
		sfp2->sz = len;
		sfp1->sz = s - sizeof(size_t);
		return &(sfp2->nx);
	}
	
	//Heap full
	arch_panic("Heap is full");
	return 0;
}

void heap_free(Heap* heap, void* p)
{
	struct __freelist *fp1, *fp2, *fpnew;
	char *cp1, *cp2, *cpnew;

	/* ISO C says free(NULL) must be a no-op */
	if (p == NULL)
		return;

	heap->deallocationCount++;

	cpnew = p;
	cpnew -= sizeof(size_t);
	fpnew = (struct __freelist *)cpnew;
	fpnew->nx = 0;

	/*
	 * Trivial case first: if there's no freelist yet, our entry
	 * will be the only one on it.  If this is the last entry, we
	 * can reduce __brkval instead.
	 */
	if (heap->Freelist == NULL) {
		heap->Freelist = fpnew;
	}

	/*
	 * Now, find the position where our new entry belongs onto the
	 * freelist.  Try to aggregate the chunk with adjacent chunks
	 * if possible.
	 */
	for (fp1 = heap->Freelist, fp2 = 0;
	     fp1;
	     fp2 = fp1, fp1 = fp1->nx) {
		if (fp1 < fpnew)
			continue;
		cp1 = (char *)fp1;
		fpnew->nx = fp1;
		if ((char *)&(fpnew->nx) + fpnew->sz == cp1) {
			/* upper chunk adjacent, assimilate it */
			fpnew->sz += fp1->sz + sizeof(size_t);
			fpnew->nx = fp1->nx;
		}
		if (fp2 == 0) {
			/* new head of freelist */
			heap->Freelist = fpnew;
			return;
		}
		break;
	}
	/*
	 * Note that we get here either if we hit the "break" above,
	 * or if we fell off the end of the loop.  The latter means
	 * we've got a new topmost chunk.  Either way, try aggregating
	 * with the lower chunk if possible.
	 */
	fp2->nx = fpnew;
	cp2 = (char *)&(fp2->nx);
	if (cp2 + fp2->sz == cpnew) {
		/* lower junk adjacent, merge */
		fp2->sz += fpnew->sz + sizeof(size_t);
		fp2->nx = fpnew->nx;
	}
}

void heap_printFreelist(Heap* heap)
{
	struct __freelist *fp1;
	int i;

	debug_printf("Allocation count: %d, DeallocationCount: %d\n", heap->allocationCount, heap->deallocationCount);

	if (!heap->Freelist) {
		debug_printf("no free list\n");
		return;
	}

	for (i = 0, fp1 = heap->Freelist; fp1; i++, fp1 = fp1->nx) {
		debug_printf("entry %d @ %x: size %x, next ",
		       i, (char *)fp1, fp1->sz);
		if (fp1->nx)
			debug_printf("%x\n", (char *)fp1->nx);
		else
			debug_printf("NULL\n");
	}
}
