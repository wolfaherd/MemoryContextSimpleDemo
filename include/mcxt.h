#pragma once
#include "MemoryContext.h"
#include "c.h"

void
MemoryContextCreate(MemoryContext * node,
					MemoryContext * parent,
					const char *name);

void *palloc(Size size);

void pfree(void *pointer);