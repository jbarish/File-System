#include "main.h"

/*
 * Justin Barish
 * Jack Kraszewski
 *
 * We pledge our honor that we have abided by the Stevens Honor System 
 */

 /*Our tree structure for the file system */
 Tree fileSystem;
 
 
 
 /*
  * Read in the directories from the fileName
  *
  * Param fileName  the file to read
  */
void readDirectories(char* fileName){
	
	/*TODO: Parse the file */
	
	
	/* TODO: for each directory, call addDirFromRoot(Tree t, char* fullName); 
	 *    This will add it to the tree in the correct spot 
	 *    Use global variable: filesystem as the tree 
	 */
}
 
/*
 * Read in the files from the fileName
 *
 * Param fileName  the file to read
 */
void readFile(char* fileName){
	/*TODO: Parse the file */
	
	
	/* TODO: for each file, call:
	 *   addFileFromRoot(Tree t, char* fullName, char* fileName, long size, long timestamp);
	 *   This will add it to the tree in the correct spot 
	 *   Use global variable: filesystem as the tree
	 */
}
 
 
int main(int argc, char *argv[]){
	
	/*extract args and error check */
	if(argc < 5){
		perror("Must provide 4 arguments");
		exit(1);
	}
	/*TODO: Extract args & set globals, error check args*/
	
	
	fileSystem = makeTree();
	
	/*TODO: Implement the following functions, and replace nulls with args*/
	readDirectories(NULL);
	readFile(NULL);
}

