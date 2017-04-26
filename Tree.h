#ifndef TREE_H
#define TREE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

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

TreeNode makeDirNode(char* dirName);
TreeNode makeFileNode(char* fName, long s, char* ts);

int countOccurance(char*, char);

TreeNode parseTree(Tree t, char* fullName);
int pathExist(Tree t, char* fullName);

void addDirFromRoot(Tree t, char* fullName);
TreeNode addFileFromRoot(Tree t, char* fullName, char* fileName, long size, char* timestamp);
TreeNode addFullFileFromRoot(Tree t, char* fullName, long size, char* timestamp);

TreeNode findFile(Tree t, char* fullName, char* fileName);

TreeNode deleteFromRoot(Tree t, char* fullName, char* deleteName);
void freeTreeNode(TreeNode t);
void disposeTree(TreeNode t);

void printPreOrder(Tree t);

#endif
