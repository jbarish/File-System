#include "FileMemory.h"
#include "LinkedList.h"
#include "LDisk.h"
#include "Tree.h"
#include "math.h"
#include "time.h"
/*
 * Justin Barish
 * Jack Kraszewski
 *
 * We pledge our honor that we have abided by the Stevens Honor System 
 */
 
 
 /*
  * Gets the current date and time as a sting
  *
  * Return  the string representation of the date and time
  */
 char* getTimeAsString(){
	time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char* str= malloc(sizeof(char)*64);
	char s[64];
    strftime(s, sizeof(s), "%c", tm);
	strcpy(str, s);
    return str;
 }
 
 
 /*
  * Will add nodes to the files memory list
  * Not for external use
  *
  * Param tn     The tree node that is the file
  * Param min    The min address of the block to be added
  * Param max	 The max address of the block to be added
  */
 void addToFileMemory(TreeNode tn, long min, long max ){
	 for(long i = min; i<max; i++){
		
		 long* temp = malloc(sizeof(long));
		 *temp = blockSize* i;
	
		 append(tn->data, temp);
	 }
 }
 
 
 
 /*
  * Adds the appropriate amount of memory for the given tree node
  * 
  * Param tn    The tree node to recieve memory
  * Param load  Whether or not to change the timestamp (if loading from file);
  */
 void allocate(TreeNode tn, LL l, int load){
	long numBlocks = (long)ceil(tn->size/(double)blockSize);
	if(!load){
		if(tn->timestamp != NULL){
			free(tn->timestamp);
		}
		tn->timestamp = getTimeAsString();
	}
	for(int i = 0; i<getNumElements(l)&& numBlocks >0; i++){
		
		Node temp = getNodeAt(l, i);
		LDisk ld = (LDisk)(temp->elem);
		if(ld->st==FREE){
		
			long blockSlots = ld->maxBlock -ld->minBlock +1;
			//	printf("blockSlots: %ld, numBlocks: %ld, size: %ld, ld->min: %ld, max:%ld\n", blockSlots, numBlocks, tn->size, ld->minBlock,ld->minBlock+numBlocks);
			if( blockSlots>= numBlocks){		
				addToFileMemory(tn, ld->minBlock, (long)ld->minBlock+numBlocks);
				requestMemory(l, i, ld->minBlock, numBlocks, USED);
				return;
			}else{
				
				addToFileMemory(tn, ld->minBlock, ld->minBlock+blockSlots);
				requestMemory(l, i, ld->minBlock, blockSlots, USED);
				
				numBlocks-= blockSlots;
			}
		}
	}
	if(numBlocks>0){
		printf("Out of space\n");
	}
	 
 }
 
 /*
  * Expand a file by a given size
  * 
  * Param tn   The TreeNode to expand to
  * Param l    The LDisk list
  * Param size The size to add to the file
  */
 void expand(TreeNode tn, LL l, long size){
	 if(tn->timestamp != NULL){
			free(tn->timestamp);
	}
	 tn->timestamp = getTimeAsString();
	 long numBytesInLastBlock = tn->size%blockSize;
	 tn->size +=size;
	 size-= blockSize-numBytesInLastBlock;
	 long numBlocks = (long)ceil(size/(double)blockSize);
	 for(int i = 0; i<getNumElements(l) && numBlocks >0; i++){
		
		Node temp = getNodeAt(l, i);
		LDisk ld = (LDisk)(temp->elem);
		if(ld->st==FREE){
		
			long blockSlots = ld->maxBlock -ld->minBlock +1;
			//	printf("blockSlots: %ld, numBlocks: %ld, size: %ld, ld->min: %ld, max:%ld\n", blockSlots, numBlocks, tn->size, ld->minBlock,ld->minBlock+numBlocks);
			if( blockSlots>= numBlocks){	
				addToFileMemory(tn, ld->minBlock, (long)ld->minBlock+numBlocks);
				requestMemory(l, i, ld->minBlock, numBlocks, USED);
				return;
			}else{
				addToFileMemory(tn, ld->minBlock, ld->minBlock+blockSlots);
				requestMemory(l, i, ld->minBlock, blockSlots, USED);
				numBlocks-= blockSlots;
			}
		}
	}
	printf("Out of space\n");
 }
 
 /*
  * Shrink a given file by a given size
  *
  * Param tn    The TreeNode corresponding to the file
  * Param l     The LDisk list
  * Param size  The amount to remove from the file
  */
 void shrink(TreeNode tn, LL l, long size){
	 if(tn->timestamp != NULL){
			free(tn->timestamp);
	}
	 tn->timestamp = getTimeAsString();
	 long numBytesInLastBlock = tn->size%blockSize;
	 tn->size -=size;
	 if( tn->size < 0){
		 printf("Cannot Remove %ld from file. Not enough memory in that file. File size is now 0.\n", size);
		 tn->size = 0;
	 }
	
	 size -= numBytesInLastBlock;
	 //long numBlocks = (long)ceil((size-numBytesInLastBlock +1)/(double)blockSize);
	 while(size>=0){
		Node temp = getLastNode(tn->data);
		long blockAddr = *((long*)(temp->elem));
		long blockID = blockAddr/blockSize;
		free(removeNode(tn->data, temp));
		requestMemory(l, check(l, &blockID), blockID, 1, FREE);
		size-=blockSize;
	 }
 }
 