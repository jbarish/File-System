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
void addFileFromRoot(Tree t, char* fullName, char* fileName, long size, long timestamp){
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
		perror("Path does not exist in current file structure. Cannot add file"), exit(1);
	}
	
	TreeNode curr = t->root;
	
	/*curr is always 1 behind i*/
	for(i = 1; i<numNames; i++){
		
		char* node = names[i];
		int nodePos = check(curr->children, node);
		
		if(nodePos==-1){
			perror("Path does not exist in current file structure. Cannot add file"), exit(1);
		}
		curr = (TreeNode)getElemAt(curr->children, nodePos);
	}
	int filePos = check(curr->children, fileName);
	if(filePos!= -1){
		/*file already exists */
		printf("File Already Exists!\n");
	}else{
		append(curr->children, makeFileNode(fileName, size, timestamp));
	}
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
		perror("Path does not exist in current file structure. Cannot find file"), exit(1);
	}
	
	TreeNode curr = t->root;
	
	/*curr is always 1 behind i*/
	for(i = 1; i<numNames; i++){
		
		char* node = names[i];
		int nodePos = check(curr->children, node);
		
		if(nodePos==-1){
			perror("Path does not exist in current file structure. Cannot find file"), exit(1);
		}
		curr = (TreeNode)getElemAt(curr->children, nodePos);
	}
	int filePos = check(curr->children, fileName);
	TreeNode res = NULL;
	if(filePos!= -1){
		res = (TreeNode)getElemAt(curr->children, filePos);
	}else{
		perror("Cannot find file"), exit(1);
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
		}else{
			
			printf("%s/%s\n", path, t->fileName);
		}
	}
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
	//printf("fileName= %s\n", findFile(t, "./B/D", "Hello.txt")->fileName);
	printPreOrder(t);
	return 0;
}
	
*/



