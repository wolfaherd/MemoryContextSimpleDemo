#pragma once

#define ALLOCSET_DEFAULT_MINSIZE   0
#define ALLOCSET_DEFAULT_INITSIZE  (8 * 1024)

#define ALLOCSET_SMALL_MINSIZE	 0
#define ALLOCSET_SMALL_INITSIZE  (1 * 1024)

static inline MemoryContext *
GetMemoryChunkContext(void *pointer)
{
	MemoryContext * context;

	/*
	 * Try to detect bogus pointers handed to us, poorly though we can.
	 * Presumably, a pointer that isn't MAXALIGNED isn't pointing at an
	 * allocated chunk.
	 */
	assert(pointer != NULL);
	assert(pointer == (void *) MAXALIGN(pointer));

	/*
	 * OK, it's probably safe to look at the context.
	 */
	context = *(MemoryContext * *) (((char *) pointer) - sizeof(void *));


	return context;
};