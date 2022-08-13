#include <iostream>
#include "MemoryContext.h"
#include "aset.h"
#include "mcxt.h"

using namespace std;

MemoryContext * TopMemoryContext = NULL;
MemoryContext * FirstContext = NULL;
MemoryContext * SecondContext = NULL;

MemoryContext * CurrentMemoryContext = NULL;

void MemoryContextInit()
{
	TopMemoryContext = AllocSetContextCreate((MemoryContext *) NULL,
											 "TopMemoryContext",
											 0,
                                             8 * 1024,
                                             8 * 1024 * 1024);

	CurrentMemoryContext = TopMemoryContext;
}

struct LargeObject{
	int i;
	long j;
	char k[10000];
};

struct SmallObject{
	int i;
	long j;
};

int main()
{
    cout<< "内存上下文Demo 开始" << endl;
    MemoryContextInit();
	FirstContext = AllocSetContextCreate(TopMemoryContext,
											 "FirstContext",
											 0,
                                             8 * 1024,
                                             8 * 1024 * 1024);
	CurrentMemoryContext = FirstContext;
	LargeObject *largeObjectPtr_1 =  reinterpret_cast<LargeObject *>(palloc(sizeof(LargeObject)));
	SmallObject *smallObjectPtr_1 = reinterpret_cast<SmallObject *>(palloc(sizeof(SmallObject)));

	SecondContext = AllocSetContextCreate(TopMemoryContext,
											 "SecondContext",
											 0,
                                             8 * 1024,
                                             8 * 1024 * 1024);
	CurrentMemoryContext = SecondContext;
	LargeObject *largeObjectPtr_2 =  reinterpret_cast<LargeObject *>(palloc(sizeof(LargeObject)));
	SmallObject *smallObjectPtr_2 = reinterpret_cast<SmallObject *>(palloc(sizeof(SmallObject)));

	pfree(largeObjectPtr_1);
	pfree(smallObjectPtr_1);
	pfree(largeObjectPtr_2);
	pfree(smallObjectPtr_2);

	cout<< "内存上下文Demo 结束" << endl;
    return 0;
}


