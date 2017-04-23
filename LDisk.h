#ifndef LDISK_H
#define LDISK_H
#include "LinkedList.h"

typedef enum {USED, FREE} statusType;

struct lDiskNode{
	long minBlock;
	long maxBlock;
	statusType st;
}; typedef struct lDiskNode* LDisk;


void requestMemory(LL l, int index, int start, int size, statusType blockStatus);
LDisk makeLD(long min, long max, statusType t);

#endif