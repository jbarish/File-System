#ifndef FILE_MEMORY_H
#define FILE_MEMORY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
#include "Tree.h"
#include "main.h"

 void allocate(TreeNode tn, LL l, int load);
 char* getTimeAsString();
 void expand(TreeNode tn, LL l, long size);
 void shrink(TreeNode tn, LL l, long size);
 int enoughMemory(LL l, long size);
#endif
