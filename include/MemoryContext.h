#pragma once
#include "c.h"


class MemoryContext {
public:
	bool		isReset;		
	Size		mem_allocated;	
	
	MemoryContext * parent;		
	MemoryContext * firstchild;
	MemoryContext * prevchild;	
	MemoryContext * nextchild;	
	const char *name;			

	void	*alloc (MemoryContext* context, Size size);
	void	free_p (MemoryContext* context, void *pointer);
};
