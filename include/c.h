#pragma once
#include <stddef.h>
#include <stdint.h>
#include <cassert>
#include <string.h>

#define assertm(exp, msg) assert(((void)msg, exp))

#define ALLOCSET_DEFAULT_MINSIZE   0
#define ALLOCSET_DEFAULT_INITSIZE  (8 * 1024)

#define ALLOCSET_SMALL_MINSIZE	 0
#define ALLOCSET_SMALL_INITSIZE  (1 * 1024)

#define ALLOC_MINBITS		3
#define ALLOCSET_NUM_FREELISTS	11
#define ALLOC_CHUNK_FRACTION	4
#define ALLOC_CHUNK_LIMIT	(1 << (ALLOCSET_NUM_FREELISTS-1+ALLOC_MINBITS))

#define MAXIMUM_ALIGNOF 8

#define TYPEALIGN(ALIGNVAL,LEN)  \
	(((uintptr_t) (LEN) + ((ALIGNVAL) - 1)) & ~((uintptr_t) ((ALIGNVAL) - 1)))
#define MAXALIGN(LEN)			TYPEALIGN(MAXIMUM_ALIGNOF, (LEN))

typedef size_t Size;
typedef unsigned int uint32;

template<typename start_type,typename val_type,typename len_type>
void MemSetAlignedTemplate(start_type start, val_type val, len_type len){
	long   *_start = reinterpret_cast<long *>(start);
	int		_val = val;
	Size	_len = len;
	const int MEMSET_LOOP_LIMIT = 1024;
	const int LONG_ALIGN_MASK = sizeof(long) - 1;
	if ((_len & LONG_ALIGN_MASK) == 0 && 
		_val == 0 && 
		_len <= MEMSET_LOOP_LIMIT && 
		MEMSET_LOOP_LIMIT != 0) 
	{ 
		long *_stop = reinterpret_cast<long *> (reinterpret_cast<char *>(_start) + _len); 
		while (_start < _stop) 
			*_start++ = 0; 
	} 
	else 
		memset(_start, _val, _len); 
}

