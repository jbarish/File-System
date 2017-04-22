#include "main.h"

/*
 * Justin Barish
 * Jack Kraszewski
 *
 * We pledge our honor that we have abided by the Stevens Honor System 
 */

 /*Our tree structure for the file system */
 Tree fileSystem;
int diskSize; /*size of disk, given as arg */
int blockSize; /*size of block, given as arg */
 
 
 /*
  * Read in the directories from the fileName
  *
  * Param fileName  the file to read
  */
void readDirs(char* fileName){
	
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
	/*TODO: Parse the file 
			File is in format: permissions	numLinks	ownerName	ownerGroup	fileSize	timeStamp	directory/FileName
			We only care about fileSize, timeStamp, and directory/FileName
			*/
	
	
	/* TODO: for each file, call:
	 *   addFullFileFromRoot(Tree t, char* fullName, long size, char* timestamp)
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
	if(atoi(argv[4]) > 0){
	  blockSize = atoi(argv[4]);
	}else{
	  perror("Must give a positive, non-zero block size ");
	  exit(1);
	}
	if(atoi(argv[3]) > 0){
	  diskSize = atoi(argv[3]);
	}else{
	  perror("Must give a positive, non-zero disk size ");
	  exit(1);
	}
	readFile(argv[1]);
	readDirs(argv[2]);
	
	fileSystem = makeTree();
}
