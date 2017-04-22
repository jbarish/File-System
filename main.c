#include "main.h"
#include "Tree.h"

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
  FILE* fp;
  char str[2048];
  char* newStr;
  
  /*read from file */
  while(fgets(str, 2048, fp) != NULL){
    if(strlen(str) > 1){/* ensure no blank lines at end of file */
      sscanf(str, "%s", newStr);

      /* TODO: for each directory, call addDirFromRoot(Tree t, char* fullName); 
       *    This will add it to the tree in the correct spot 
       *    Use global variable: fileSystem as the tree 
       */
      addDirFromRoot(fileSystem, newStr);
    }
  }
  fclose(fp);
}
 
/*
 * Read in the files from the fileName
 *
 * Param fileName  the file to read
 */
void readFile(char* fileName){
  /*TODO: Parse the file 
    File is in format: 
    permissions	
    numLinks	
    ownerName	
    ownerGroup	
    fileSize	
    timeStamp	
    directory/FileName
    We only care about fileSize, timeStamp, and directory/FileName
    need int, string, and string
  */
  FILE* fp;
  char str[2048];/*entire string */
  long fstNum;
  long sndNum;
  char* perm;
  long thdNum;
  char* nameOne;
  char* nameTwo;
  long fileSize;
  char timeStampM[7];
  char timeStampN[5];
  char timeStampTwo[13];
  char fName[1024];/*if segfault malloc for char */

  /* read from file */
  while(fgets(str, 2048, fp) != NULL){
    if(strlen(str) > 1){ /*ensure no blank lines at end of file */
      sscanf(str, "%ld %ld %s %ld %s %s %ld %s %s %s %s", &fstNum, &sndNum, perm, &thdNum, nameOne, nameTwo, &fileSize, timeStampM, timeStampN, timeStampTwo, fName); /*format input string into temp */
      
      /* TODO: for each file, call:
       *   addFullFileFromRoot(Tree t, char* fullName, long size, char* timestamp)
       *   This will add it to the tree in the correct spot 
       *   Use global variable: fileSystem as the tree
       */
      char* timeStampF = (char*)malloc(sizeof(char) * 35);
      timeStampF[0] = '\0';
      strcat(timeStampF, timeStampM);
      strcat(timeStampF, timeStampN);
      strcat(timeStampF, timeStampTwo);
      addFullFileFromRoot(fileSystem, fileName, fileSize, timeStampF);
      
    }
  }
  fclose(fp);
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
	fileSystem = makeTree();
	readFile(argv[1]);
	readDirs(argv[2]);
	
}
