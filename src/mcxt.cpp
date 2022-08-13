#include "MemoryContext.h"
#include "mcxt.h"
#include "memutils.h"
#include <iostream>

extern MemoryContext* CurrentMemoryContext;

using namespace std;

void MemoryContextCreate(MemoryContext * node,
					MemoryContext * parent,
					const char *name)
{
	node->isReset = true;
	// node->methods = methods;
	node->parent = parent;
	node->firstchild = NULL;
	node->mem_allocated = 0;
	node->prevchild = NULL;
	node->name = name;
	if (parent)
	{
		node->nextchild = parent->firstchild;
		if (parent->firstchild != NULL)
			parent->firstchild->prevchild = node;
		parent->firstchild = node;
	}
	else
	{
		node->nextchild = NULL;
	}
}

void *palloc(Size size)
{
	MemoryContext * context = CurrentMemoryContext;

	context->isReset = false;

	void* ret = context->alloc(context, size);
	if (ret == NULL)
	{
		cerr<<"out of memory"<<
			"Failed on request of size "
			<<size<<" in memory context \""
			<<context->name<<"\".";
	}

	return ret;
}

void pfree(void *pointer)
{
	MemoryContext * context = GetMemoryChunkContext(pointer);

	context->free_p(context, pointer);
}