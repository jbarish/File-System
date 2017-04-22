#ifndef LDISK_H
#define LDISK_H
#include "LinkedList.h"

typedef enum {USED, FREE} statusType;

struct lDiskNode{
	int minBlock;
	int maxBlock;
	statusType st;
}; typedef struct lDiskNode* LDisk;


void requestMemory(LL l, int index, int start, int size, statusType blockStatus);
LDisk makeLD(int min, int max, statusType t);

#endif