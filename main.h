#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {TREE, LFILE} listType; 



struct linkedlist {
	struct node* head;
	struct node* tail;
	int numElements;
	listType t;
}; typedef struct linkedlist* LL;

struct tree {
	struct treeNode* root;
}; typedef struct tree* Tree;

struct treeNode{
	char* dir;
	LL children;
	
	char* fileName;
	long size;
	char* timestamp;
	LL data;
}; typedef struct treeNode* TreeNode;


Tree makeTree();
void addDirFromRoot(Tree t, char* fullName);
void printPreOrder(Tree t);
void printTnode(LL q);
TreeNode addFullFileFromRoot(Tree t, char* fullName, long size, char* timestamp);
TreeNode addFileFromRoot(Tree t, char* fullName, char* fileName, long size, char* timestamp);

LL makeLL(listType ty);
void* removeAt(LL list, int pos);
void addAt(LL list, int pos, void* elem);
void append(LL list, void* elem);
int getNumElements(LL l);
int isEmpty(LL l);
int check(LL q, void* elem );

void* getElemAt(LL l, int i);
void disposeLL(LL q);
#endif