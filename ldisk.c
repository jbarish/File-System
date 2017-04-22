#include "LDisk.h"
#include "LinkedList.h"

/*
 * Justin Barish
 * Jack Kraszewski
 *
 * We pledge our honor that we have abided by the Stevens Honor System 
 */
 
/*
 * Checks if the two nodes' elements have the same status value
 * If they do, merge them. Always will remove Node a, never Node b (if neccessary)
 * Not for external use.
 * 
 * Param l    The linked list
 * Param a    The first node
 * Param b    The second node
 */
 
void merge(LL l, Node a, Node b){
	if(a !=NULL && b!=NULL){
		LDisk la = (LDisk)a->elem;
		LDisk lb = (LDisk)b->elem;
		if(la->st == lb->st ){
			int newMin = la->minBlock < lb->minBlock ? la->minBlock :lb->minBlock ;
			int newMax = la->maxBlock > lb->maxBlock ? la->maxBlock :lb->maxBlock ;
			lb->minBlock = newMin;
			lb->maxBlock = newMax;
			free(removeNode(l, a));
		}
	}
}

/*
 * DEPRICATED!
 *
 * Will check the nodes adjacent to the one at the given index.
 * Will merge if applicable.
 * 
 *
 * Param l     The linked list
 * Param index The index of the Node
 */
void mergeAdjacentNodes(LL l, int index){
	if(l->t == LDISK){
		Node curr = getNodeAt(l, index);
		if(curr!=NULL){
			Node left = getNodeAt(l, index-1);
			Node right = getNodeAt(l, index+1);
			merge(l, left, curr);
			merge(l, right, curr);
		}
	}
}

/*
 * Will split a given node in two, given a specific size
 * Will merge if applicable.
 *
 * Param l            The linked list
 * Param index        The index of the Node
 * Param size         The size the left block needs to become
 * Param fBlockStatus The status the left block should become
 * Param sBlockStatus The status the right block should become
 */
void requestMemory(LL l, int index, int start, int size, statusType blockStatus){
	if(l->t == LDISK){
		LDisk curr = (LDisk)getElemAt(l,index);
		int newFirstMin = curr->minBlock;
		int newFirstMax = start-1;
		int newSecondMin = start;
		int newSecondMax = newSecondMin+size-1;
		int newThirdMin = newSecondMax+1;
		int newThirdMax = curr->maxBlock;
		statusType newThirdStatus = curr->st;
		
		curr->minBlock = newFirstMin;
		curr->maxBlock = newFirstMax;
		
		if(newFirstMax - newFirstMin<0){
			curr->st = blockStatus;
		}
		
		if(newThirdMax - newThirdMin<0){
			newThirdStatus = blockStatus;
		}
		
		/* Make the three new blocks from the split
		 * Extranious blocks will be removed with the merge calls
		 */
		LDisk newSNode = malloc(sizeof(struct lDiskNode));
		newSNode->minBlock = newSecondMin;
		newSNode->maxBlock = newSecondMax;
		newSNode->st = blockStatus;
		
		LDisk newHNode = malloc(sizeof(struct lDiskNode));
		newHNode->minBlock = newThirdMin;
		newHNode->maxBlock = newThirdMax;
		newHNode->st = newThirdStatus;
		
		addAt(l, index+1, newHNode);
		addAt(l, index+1, newSNode);
	
		Node LL = getNodeAt(l,index-1);
		Node L = getNodeAt(l,index);
		Node M = getNodeAt(l,index+1);
		Node R = getNodeAt(l,index+2);
		Node RR = getNodeAt(l,index+3);
		
		
		
		/*check if merging needs to be done */
		merge(l, LL, L);
		merge(l, L, M);
		merge(l, M, R);
		merge(l, RR, R);
	}
}

LDisk makeLD(int min, int max, statusType t){
	LDisk ld = malloc(sizeof(struct lDiskNode));
	ld->minBlock = min;
	ld->maxBlock = max;
	ld->st = t;
	return ld;
}

int main(){
	LL l = makeLL(LDISK);
	
	
	append(l, makeLD(0,100,FREE));
	printLDnode(l);
	requestMemory(l, 0,0, 10, USED);
	printLDnode(l);
	requestMemory(l, 1, 13,11, USED);
	printLDnode(l);	
	requestMemory(l, 1, 11,1, USED);
	printLDnode(l);	
	requestMemory(l, 0, 5,1, FREE);
	printLDnode(l);	
		
		
	requestMemory(l, 1, 5,1, USED);
	printLDnode(l);	
	
	requestMemory(l, 1, 10,1, USED);
	printLDnode(l);	
	return 0;
}
