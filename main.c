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
  char newStr[2048];
  //char* newStr = (char*)malloc(sizeof(char)*2048);
  newStr[0] = '\0';
  fp = fopen(fileName, "r");

  //printf("2: defs\n");
  
  /*read from file */
  while(fgets(str, 2048, fp) != NULL){
    if(strlen(str) > 1){/* ensure no blank lines at end of file */
      
     // printf("2: before thing\n");
      
      sscanf(str, "%s", newStr);
      
      //printf("2: looped\n");

      /* TODO: for each directory, call addDirFromRoot(Tree t, char* fullName); 
       *    This will add it to the tree in the correct spot 
       *    Use global variable: fileSystem as the tree 
       */
      addDirFromRoot(fileSystem, newStr);
     // printf("2: added\n");
    }
  }
  //free(newStr);
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
  char perm[10];
  long thdNum;
  char nameOne[1024];
  char nameTwo[1024];
  long fileSize;
  char timeStampM[7];
  char timeStampN[5];
  char timeStampTwo[13];
  char fName[1024];
  fp = fopen(fileName, "r");
//  printf("3: defs\n");

  /* read from file */
  while(fgets(str, 2048, fp) != NULL){
    if(strlen(str) > 1){ /*ensure no blank lines at end of file */
      sscanf(str, "%ld %ld %s %ld %s %s %ld %s %s %s %[^\n]", &fstNum, &sndNum, perm, &thdNum, nameOne, nameTwo, &fileSize, timeStampM, timeStampN, timeStampTwo, fName); /*format input string into temp */
      //printf("3: read\n");
   //   printf("%ld, %ld, %s, %ld, %s, %s, %ld, %s, %s, %s, %s\n",fstNum,sndNum, perm, thdNum, nameOne, nameTwo, fileSize, timeStampM, timeStampN, timeStampTwo, fName);
      /* TODO: for each file, call:
       *   addFullFileFromRoot(Tree t, char* fullName, long size, char* timestamp)
       *   This will add it to the tree in the correct spot 
       *   Use global variable: fileSystem as the tree
       */
      char* timeStampF = (char*)malloc(sizeof(char) * 35);
      timeStampF[0] = '\0';
      strcat(timeStampF, timeStampM);
	   strcat(timeStampF, " ");
      strcat(timeStampF, timeStampN);
	    strcat(timeStampF, " ");
      strcat(timeStampF, timeStampTwo);
     // printf("3: made timeStamp\n");
	//  printf("fileName=%s, ts = %s\n", fName, timeStampF);
      addFullFileFromRoot(fileSystem, fName, fileSize, timeStampF);
      //printf("3: added\n");
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
	//printf("first\n");
	fileSystem = makeTree();
	//printf("second\n");
	readDirs(argv[2]);
	//printf("third\n");
	readFile(argv[1]);
	printPreOrder(fileSystem);
	return 0;
}
