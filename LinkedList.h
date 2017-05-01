#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {TREE, LFILE, LDISK} listType; 

struct linkedlist {
	struct node* head;
	struct node* tail;
	int numElements;
	listType t;
}; 
typedef struct linkedlist* LL;

struct node{
	void* elem;
	struct node* next;
	struct node* prev;
};
typedef struct node* Node;


LL makeLL(listType ty);

void printTnode(LL q);
void printTInfonode(LL q);
void printLDnode(LL q);
void printLLInt(LL q);

void* removeNode(LL list, Node n);
void* removeAt(LL list, int pos);

void addAt(LL list, int pos, void* elem);
void append(LL list, void* elem);

int getNumElements(LL l);
int isEmpty(LL l);

int check(LL q, void* elem );
void* getElemAt(LL l, int pos);
void* getNodeAt(LL l, int pos);
void* getLastNode(LL l);

void disposeLL(LL q);

#endif
