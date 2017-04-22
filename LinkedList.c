#include "LinkedList.h"
#include "Tree.h"
#include "LDisk.h"

/*
 * Justin Barish
 * Jack Kraszewski
 *
 * We pledge our honor that we have abided by the Stevens Honor System 
 */


/*
 *  [head]                   [tail]
 *     |                       |
 *  |elem     |   |elem   |   |elem     |
 *  |prev=null|<--|=prev  |<--|=prev    |
 *  |next = --|-->|next=--|-->|next=null|
 *
 * Add to head
 */

/*Initialize the LL
 * 
 * Return         the newly malloc'ed LL
 */
LL makeLL(listType ty){
	LL q =  (LL)malloc(sizeof(struct linkedlist));
	q->numElements = 0;
	q->head = NULL;
	q->t = ty;
	return q;
}

/*
 * Debugging Only: Prints all elements in the List from head to tail
 * ALERT: Elements MUST be a STRING to use this function
 * 
 * Param q  The LinkedList
 */
void printLLString(LL q){
	Node curr = q->head;
	while(curr!=NULL){
		printf("%s,", (char*)curr->elem);
		curr = curr->next;
	}
	printf("\n");
}



/*
 * Debugging Only: Prints all elements in the List from tail to head
 * ALERT: Elements MUST be a STRING to use this function
 * 
 * Param q  The LinkedList
 */
void printRLLString(LL q){
	Node curr = q->tail;
	while(curr!=NULL){
		printf("%s,", (char*)curr->elem);
		curr = curr->prev;
	}
	printf("\n");
}

/*
 * Debugging Only: Prints all elements in the List from head to tail
 * ALERT: Elements MUST be a TREENODE to use this function
 * 
 * Param q  The LinkedList
 */
void printTnode(LL q){
	Node curr = q->head;
	while(curr!=NULL){
		printf("%s,", ((TreeNode)curr->elem)->dir);
		curr = curr->next;
	}
	printf("\n");
}

/*
 * Debugging Only: Prints all elements in the List from head to tail
 * ALERT: Elements MUST be a LDisk to use this function
 * 
 * Param q  The LinkedList
 */
void printLDnode(LL q){
	Node curr = q->head;
	while(curr!=NULL){
		printf("Min:%i, Max:%i, Status: %s ||", ((LDisk)curr->elem)->minBlock, 
		((LDisk)curr->elem)->maxBlock, ((LDisk)curr->elem)->st== FREE ? "FREE" : "USED");
		curr = curr->next;
	}
	printf("\n");
}

/*
 * Remove a node
 *
 * Param LL  The LinkedList
 * Param n   The node to remove
 *
 * Return    A void pointer to the element that was removed
 */
void* removeNode(LL list, Node n){
	
	Node curr= n;
	void* temp = curr->elem;
	
	/*if was first thing in list, reset head. Otherwise, relink nodes*/
	if(curr->prev==NULL){
		list->head = curr->next;
	}else{
		curr->prev->next = curr->next;
	}
	
	/*if was last thing in list, reset tail */
	if(curr->next == NULL){
		list->tail = curr->prev;
	}else{
		curr->next->prev = curr->prev;
	}
	
	free(curr);
	list->numElements--;
	
	return temp;
}


/*
 * Remove a node at a specified position
 *
 * Param LL  The LinkedList
 * Param pos The position in the linked list to remove from
 *           Will clamp pos to [0,inf)
 *
 * Return    A void pointer to the element that was removed
 */
void* removeAt(LL list, int pos){
	
	/*make pos start at 0 */
	if(pos<0) {
		pos=0;
	}
	
	/*check that there is at least 1 thing in the list */
	if(list->head!= NULL){
		int i;
		Node curr= list->head;
		for( i=0; i<list->numElements; i++){
			if(i==pos){
				return removeNode(list, curr);
			}
			curr = curr->next;
		}
	}
	/*if nothing was removed*/
	return NULL;
}


/*
 * Add a node at a specified position
 *
 * Param LL    The LinkedList
 * Param pos   The position in the linked list to add to
 *             Will clamp pos to [0,list length]
 * Param elem  The element to add to the list
 *
 */
void addAt(LL list, int pos, void* elem){
	
	Node n = (Node)malloc(sizeof(struct node));
	n->elem = elem;
	n->next = NULL;
	n->prev = NULL;
	
	/*if there is already stuff in the list*/
	if(list->head!= NULL){
		int i;
		Node curr= list->head;
		
		/*clamp pos between 0 and list length */
		if(pos<0) {
			pos=0;
		}
		if(pos>list->numElements) {
			pos = list->numElements;
		}
		
		for( i=0; i<list->numElements; i++){
			if(i==pos){
				/*if is first thing in list, reset head*/
				if(curr->prev==NULL){
					list->head = n;
				}else{
					curr->prev->next = n;
				}
				n->next = curr;
				n->prev = curr->prev;
				curr->prev = n;
				
				list->numElements++;
				return;
			}
			curr = curr->next;
		}
		/*adding to end*/
		list->tail->next = n;
		n->prev = list->tail;
		list->tail = n;
		list->numElements++;
	}else{
		/*adding first thing in list*/
		list->head = n;
		list->tail = n;
		list->numElements++;
	}
}

/*
 * Add a node at the end of the list
 *
 * Param LL    The LinkedList
 * Param elem  The element to add to the list
 *
 */
void append(LL list, void* elem){
	addAt(list, list->numElements, elem);
}

/*
 * Returns the length of the list
 *
 * Param LL    The LinkedList
 *
 */
int getNumElements(LL l){
	return l->numElements;
}

/*
 * Check if the list is empty
 *
 * Param LL    The LinkedList
 *
 */
int isEmpty(LL l){
	return l->numElements==0 ? 1 : 0;
}

/* 
 * Checks if given element is in the LL
 * Uses the type specified of the list to determine how to parse void* elem
 *
 * Param q     the LL
 * Param elem  the id to check for
 * Return 	   the position the element is in the list. Return -1 if not in the list
 */
int check(LL q, void* elem ){
	Node curr = q->head;
	int counter = 0;
	while(curr!=NULL){
		switch(q->t){
			case TREE:
			{
				char* tn = (char*)elem;
				TreeNode c = (TreeNode)(curr->elem);				
				if(c->dir!=NULL && strcmp(tn,c->dir)==0 ||
					(c->fileName!=NULL && strcmp(tn,c->fileName)==0 )){
					return counter;
				}
				break;
			}
		}
		curr = curr->next;
		counter++;
	}
	/*not found in list */
	return -1;
}

/*
 * get the element at the ith index from the LinkedList
 *
 * Param LL    The LinkedList
 * Param pos   The position in the linked list to get the element from
 *             Will clamp pos to [0,list length]
 */
void* getElemAt(LL l, int pos){
	int i;
	Node curr= l->head;
	if(pos<0) {
		pos=0;
	}
	if(pos>l->numElements) {
		pos = l->numElements;
	}
	
	for( i=0; i<l->numElements && curr!=NULL; i++){
		if(i==pos){
			break;
		}
		curr = curr->next;
	}
	return curr->elem;
}

/*
 * get the node at the ith index from the LinkedList
 * Not for external use
 *
 * Param LL    The LinkedList
 * Param pos   The position in the linked list to get the element from
 *             Will clamp pos to [0,list length]
 */
void* getNodeAt(LL l, int pos){
	int i;
	Node curr= l->head;
	if(pos<0) {
		return NULL;
	}
	if(pos>l->numElements) {
		return NULL;
	}
	
	for( i=0; i<l->numElements && curr!=NULL; i++){
		if(i==pos){
			break;
		}
		curr = curr->next;
	}
	return curr;
}



/* 
 * Delete all nodes in the List
 *
 * Param q  the List
 */
void disposeLL(LL q){
	Node curr = q->head;
	while(curr!=NULL){
		Node tmp = curr->next;
		free(curr);
		curr = tmp;
		q->numElements--;
	}
	q->head = NULL;
	q->tail = NULL;
}

/*
int main(){
	LL l = makeLL(LDISK);
	
	addAt(l, 0, "s");
	addAt(l, 0, "t");
	addAt(l, 3, "v");
	addAt(l, 3, "w");
	addAt(l, 1, "a");
	addAt(l, 10, "b");
	printLLString(l);
	printRLLString(l);
	printf("%s\n", (char*) removeAt(l, -1));
	printLLString(l);
	printRLLString(l);
	printf("%s\n", (char*)removeAt(l, 4));
	printLLString(l);
	printRLLString(l);
	printf("%s\n", (char*)removeAt(l, 1));
	printLLString(l);
	printRLLString(l);
	printf("%s\n", (char*)removeAt(l, 1));
	printLLString(l);
	printRLLString(l);
	printf("%s\n", (char*)removeAt(l, 1));
	printLLString(l);
	printRLLString(l);
	printf("%s\n", (char*)removeAt(l, 0));
	printLLString(l);
	printRLLString(l);
	addAt(l, 0, "s");
	printLLString(l);
	printRLLString(l);

	disposeLL(l);
	printLLString(l);
	

}
*/

