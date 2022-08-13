#include "stdlib.h"
#include "aset.h"
#include "MemoryContext.h"
#include "c.h"
#include "memutils.h"
#include "mcxt.h"
#include <iostream>
#include <algorithm>

using namespace std;

struct AllocSetFreeList
{
	int	num_free;		/* current list length */
	AllocSetContext *first_free;	/* list header */
};

AllocSetFreeList context_freelists[2] =
{
	{
		0, NULL
	},
	{
		0, NULL
	}
};

MemoryContext * AllocSetContextCreate(MemoryContext * parent,
							  const char *name,
							  Size minContextSize,
							  Size initBlockSize,
							  Size maxBlockSize)
{
	cout << "AllocSetContextCreate begin" << endl;
	int			freeListIndex;
	Size		firstBlockSize;
	AllocSet	set;
	AllocBlock	block;

	if (minContextSize == ALLOCSET_DEFAULT_MINSIZE &&
		initBlockSize == ALLOCSET_DEFAULT_INITSIZE)
		freeListIndex = 0;
	else if (minContextSize == ALLOCSET_SMALL_MINSIZE &&
			 initBlockSize == ALLOCSET_SMALL_INITSIZE)
		freeListIndex = 1;
	else
		freeListIndex = -1;

	/*
	 * If a suitable freelist entry exists, just recycle that context.
	 */
	if (freeListIndex >= 0)
	{
		AllocSetFreeList *freelist = &context_freelists[freeListIndex];

		if (freelist->first_free != NULL)
		{
			/* Remove entry from freelist */
			set = freelist->first_free;
			// freelist->first_free = (AllocSet) set->header.nextchild;
			freelist->first_free = (AllocSet) set->header.nextchild;
			freelist->num_free--;

			/* Update its maxBlockSize; everything else should be OK */
			set->maxBlockSize = maxBlockSize;

			/* Reinitialize its header, installing correct name and parent */
			MemoryContextCreate((MemoryContext *) set,
								parent,
								name);

			((MemoryContext *) set)->mem_allocated =
				set->keeper->endptr - ((char *) set);

			return (MemoryContext *) set;
		}
	}

	firstBlockSize = MAXALIGN(sizeof(AllocSetContext)) +
		ALLOC_BLOCKHDRSZ + ALLOC_CHUNKHDRSZ;
	if (minContextSize != 0)
		firstBlockSize = std::max(firstBlockSize, minContextSize);
	else
		firstBlockSize = std::max(firstBlockSize, initBlockSize);

	set = (AllocSet) malloc(firstBlockSize);
	if (set == NULL)
	{
		cerr<<"out of memory Failed while creating memory context "<<name<<endl;
	}

	block = (AllocBlock) (((char *) set) + MAXALIGN(sizeof(AllocSetContext)));
	block->aset = set;
	block->freeptr = ((char *) block) + ALLOC_BLOCKHDRSZ;
	block->endptr = ((char *) set) + firstBlockSize;
	block->prev = NULL;
	block->next = NULL;

	set->blocks = block;
	set->keeper = block;

	MemSetAlignedTemplate(set->freelist, 0, sizeof(set->freelist));

	set->initBlockSize = initBlockSize;
	set->maxBlockSize = maxBlockSize;
	set->nextBlockSize = initBlockSize;
	set->freeListIndex = freeListIndex;

	set->allocChunkLimit = ALLOC_CHUNK_LIMIT;
	while ((Size) (set->allocChunkLimit + ALLOC_CHUNKHDRSZ) >
		   (Size) ((maxBlockSize - ALLOC_BLOCKHDRSZ) / ALLOC_CHUNK_FRACTION))
		set->allocChunkLimit >>= 1;

	/* Finally, do the type-independent part of context creation */
	MemoryContextCreate((MemoryContext *) set,
						parent,
						name);

	((MemoryContext *) set)->mem_allocated = firstBlockSize;

	cout << "AllocSetContextCreate end" << endl;
	return (MemoryContext *) set;
}