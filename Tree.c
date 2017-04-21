/*
 * Justin Barish
 * Jack Kraszewski
 *
 * We pledge our honor that we have abided by the Stevens Honor System 
 */
 
 #include "main.h"
 

/*Initialize the tree
 * 
 * Return         the newly malloc'ed tree 
 */
Tree makeTree(){
	Tree t =  (Tree)malloc(sizeof(struct tree));
	t->root = NULL;
	return t;
}


/*
 * Count the number of occurances of a specific character in the string
 * For local use only
 *
 * Param fullName   The string to parse
 * Param c          The character to countOccurance
 */
int countOccurance(char* fullName, char c){
	int counter = 0;
	for(int i = 0; i<strlen(fullName); i++){
		if(fullName[i]==c){
			counter++;
		}
	}
	return counter;
}

/*
 * Make a Directory node
 *
 * Param dirName  The name of the directory (not the full name)
 * Return         The newly made TreeNode
 */
TreeNode makeDirNode(char* dirName){
	TreeNode d = (TreeNode)malloc(sizeof(struct treeNode));
	d->dir = malloc(sizeof(char)*strlen(dirName));
	
	strcpy(d->dir,dirName);

	d->children=makeLL(TREE);
	d->fileName = NULL;
	d->size = -1;
	d->timestamp =-1;
	d->data = NULL;
	return d;
}

/*
 * Make a File node
 *
 * Param fileName  The name of the file
 * Param s         The size of the file
 * Param ts        The timestamp of the file
 * Return          The newly made TreeNode
 */
TreeNode makeFileNode(char* fName, long s, long ts){
	TreeNode d = (TreeNode)malloc(sizeof(struct treeNode));
	
	d->dir = NULL;
	d->children=NULL;
	
	d->fileName = malloc(sizeof(char)*strlen(fName));
	strcpy(d->fileName,fName);
	d->size = s;
	d->timestamp =ts;
	d->data = makeLL(LFILE);
	return d;
}

/*
 * Add a directory folder
 * 
 * Param t         The tree structure
 * Param fullName  The FULLNAME of the directory, starting from the root name
 */
void addDirFromRoot(Tree t, char* fullName){
	int numNames = countOccurance(fullName, '/') +1; 
	char** names = (char**)malloc(sizeof(char*)*numNames);
	int counter = 0;
	int i=0;
	char* tempName = (char*)malloc(sizeof(char)*strlen(fullName));
	strcpy(tempName, fullName);
	
	/*turn the name string into an array of individual directory names */
	char* s = strtok(tempName,"/");
	
	while(s!=NULL){
		counter++;
		names[i++] = s;
		s = strtok(NULL,"/");
	}
	numNames = counter; /*handle case of root directory ./ */
	if(t->root==NULL){
		t->root = makeDirNode(names[0]);
	}
	TreeNode curr = t->root;
	
	
	/*curr is always 1 behind i*/
	for(i = 1; i<numNames; i++){
		
		char* node = names[i];
		int nodePos = check(curr->children, node);
		
		if(nodePos==-1){
			append(curr->children, makeDirNode(names[i]));
		}
		nodePos = check(curr->children, node);
		if(nodePos==-1){
			printf("Error Code:%i\n", i);
			perror("something wrong with the tree"), exit(1);
		}
		curr = (TreeNode)getElemAt(curr->children, nodePos);
		
	}
	free(names);
	free(tempName);
}

/*
 * Given a tree, traverses it according to the path specified in fullName
 * 
 * Param t        The tree to parse
 * Param fullName The full name of the directory, starting from root
 * Return         Pointer to the last specified directory
 */
TreeNode parseTree(Tree t, char* fullName){
	int numNames = countOccurance(fullName, '/') +1; 
	char** names = (char**)malloc(sizeof(char*)*numNames);
	int i=0;
	int counter = 0;
	char* tempName = (char*)malloc(sizeof(char)*strlen(fullName));
	strcpy(tempName, fullName);
	char* s = strtok(tempName,"/");
	
	while(s!=NULL){
		counter++;
		names[i++] = s;
		s = strtok(NULL,"/");
	}
	numNames = counter; /*handle case of root directory ./ */
	
	if(t->root==NULL){
		perror("Path does not exist in current file structure."), exit(1);
	}
	
	TreeNode curr = t->root;
	
	/*curr is always 1 behind i*/
	for(i = 1; i<numNames; i++){
		
		char* node = names[i];
		int nodePos = check(curr->children, node);
		
		if(nodePos==-1){
			perror("Path does not exist in current file structure. "), exit(1);
		}
		curr = (TreeNode)getElemAt(curr->children, nodePos);
	}
	free(names);
	free(tempName);
	return curr;
}


/*
 * Add a file to the tree
 * 
 * Param t         The tree structure
 * Param fullName  The FULLNAME of the directory, starting from the root name
 * Param fileName  The name of the file
 * Param size      The size of the file
 * Param timestamp The timestamp of the file
 */
TreeNode addFileFromRoot(Tree t, char* fullName, char* fileName, long size, long timestamp){
	TreeNode curr = parseTree(t, fullName);
	int filePos = check(curr->children, fileName);
	if(filePos!= -1){
		/*file already exists */
		printf("File Already Exists!\n");
		return NULL;
	}else{
		TreeNode temp = makeFileNode(fileName, size, timestamp);
		append(curr->children, temp);
		return temp;
	}
}
/*
 * Add a file to the tree
 * 
 * Param t         The tree structure
 * Param fullName  The FULLNAME of the directory and file, starting from the root name
 * Param size      The size of the file
 * Param timestamp The timestamp of the file
 */
TreeNode addFullFileFromRoot(Tree t, char* fullName, long size, long timestamp){
	char* fName = strrchr(fullName, '/') +1;
	int loc = (int)(fName - fullName-1);
	char* path= malloc(sizeof(loc+1));
	strncpy(path, fullName, loc);
	path[loc] = '\0';
	printf("Path = %s, Fname = %s\n", path, fName);
	TreeNode temp =  addFileFromRoot( t, path, fName,  size,  timestamp);
	free(path);
	return temp;

}

/*
 * FInd a file in the tree
 * 
 * Param t         The tree structure
 * Param fullName  The FULLNAME of the directory, starting from the root name
 * Param fileName  The name of the file
 * Return          The pointer to the tree node that contains that file
 */
TreeNode findFile(Tree t, char* fullName, char* fileName){
	TreeNode curr = parseTree(t, fullName);
	int filePos = check(curr->children, fileName);
	TreeNode res = NULL;
	if(filePos!= -1){
		res = (TreeNode)getElemAt(curr->children, filePos);
	}else{
		printf("Cannot find file\n");
	}
	return res;
}

/* 
 * Delete a given file or directory
 *
 * Param t          The tree structure
 * Param fullName   The full path, starting from root
 * Param deleteName The file/directory to deleteFromRoot
 */
TreeNode deleteFromRoot(Tree t, char* fullName, char* deleteName){
	TreeNode curr = parseTree(t, fullName);
	int filePos = check(curr->children, deleteName);
	TreeNode res = NULL;
	if(filePos!= -1){
		res = (TreeNode)getElemAt(curr->children, filePos);
		if((res->children != NULL && isEmpty(res->children)) || res->data!=NULL){
			removeAt(curr->children, filePos);
		}else if(res->children != NULL && !isEmpty(res->children)){
			printf("Directory is not empty\n");
			return NULL;
		}
	}else{
		printf("Cannot find file/directory");
	}
	return res;
}


/*
 * Preorder helper function
 * Not for external use
 */
void preOrder(TreeNode t, char* path, int root){
	if(t!=NULL){
		if(t->dir!=NULL){
			
			char* str = (char*)malloc(sizeof(char)*(strlen(path) + strlen(t->dir))+1);
			str[0] = '\0';
			strcat(str, path);
			if(strlen(path)!=0){
				strcat(str, "/");	
			}
			strcat(str, t->dir);
			printf("%s", str);
			if(root){
				printf("/");
			}
			printf("\n");
			for(int i = 0; i< getNumElements(t->children); i++){
				preOrder((TreeNode)getElemAt(t->children, i), str,0);
			}
			free(str);
		}else{
			printf("%s/%s\n", path, t->fileName);
		}
	}
}

/*
 * Frees all internal data of a tree node
 * ALERT! Also frees the passed in tree node!!!
 *
 * Param t   the TreeNode to be freed
 */
void freeTreeNode(TreeNode t){
	if(t->dir!=NULL){
		free(t->dir);
	}
	if(t->children!=NULL){
		disposeLL(t->children);
		free(t->children);
	}
	if(t->fileName!=NULL){
		free(t->fileName);
	}
	t->size = 0;
	t->timestamp=0;
	if(t->data!=NULL){
		disposeLL(t->data);
		free(t->data);
	}
	free(t);
}

/*
 * Print the tree, in preorder
 * 
 * Param t  the tree to print
 */
void printPreOrder(Tree t){
	TreeNode start = t->root;
	char* str = malloc(sizeof(char)+1);
	str[0] = '\0';
	/*print the root*/
	preOrder(start, str,1);
}

/*
int main(){
	Tree t = makeTree();
	addDirFromRoot( t, "./");
	//printPreOrder(t);
	addDirFromRoot( t, "./A");
	//printPreOrder(t);
	addDirFromRoot( t, "./B");
	//printPreOrder(t);
	addDirFromRoot( t, "./B/C");
	//printPreOrder(t);
	addDirFromRoot( t, "./B/D");
	//printPreOrder(t);
	addFileFromRoot( t, "./B/D", "Hello.txt", 0, 0);
	
	addFileFromRoot( t, "./", "B.txt", 0, 0);
	addFullFileFromRoot(t, "./B/e.txt", 0, 0);
	
	printPreOrder(t);
	TreeNode temp = deleteFromRoot(t, "./B", "C");
	printf("dfileName= %s\n",temp->dir);
	freeTreeNode(temp);
	temp =findFile(t, "./B" , "e.txt");
	printf("fileName= %s\n", temp->fileName);
	
	printPreOrder(t);
	return 0;
}
*/




