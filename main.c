#include "main.h"
#include "Tree.h"
#include "LinkedList.h"
#include "LDisk.h"
#include "FileMemory.h"
/*
 * Justin Barish
 * Jack Kraszewski
 *
 * We pledge our honor that we have abided by the Stevens Honor System 
 */

/*Our tree structure for the file system */
Tree fileSystem;
long diskSize; /*size of disk, given as arg */

LL lDiskList;
 
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
		
      allocate(addFullFileFromRoot(fileSystem, fName, fileSize, timeStampF), lDiskList, 1);
	  
      //printf("3: added\n");
    }
  }
  fclose(fp);
}
 
 
int main(int argc, char *argv[]){
  /*variables*/
  char* buffer;
  int n = 0;
  int counter = 0;
  char* temp;
  int sizeToRead = 1024;
  int first = 1;
  buffer = (char*)malloc(1024);
  
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
  lDiskList = makeLL(LDISK);
  
  
  
  
  append(lDiskList, makeLD(0, (int)(diskSize-1)/blockSize, FREE));
  
  //printf("second\n");
  readDirs(argv[2]);
  //printf("third\n");
  printf("Reading in files. Please Wait...\n");
  readFile(argv[1]);
  //printLDnode(lDiskList);
  //printPreOrder(fileSystem);

  //handle inputs replace fgets with getline
  while(1){
    sizeToRead = 1024;
    while(scanf("%[^\n]", buffer)){
      if(*(buffer - 1) == '\n'){
	*(buffer - 1) = '\0';
	break;
      }
    }
    buffer[n-1] = '\0';
    buffer -= counter;
    if(n >= 0){
      if(strcmp(buffer, "exit") == 0){
	printf("exit\n");
      }else if(strncmp(buffer, "cd", 3) == 0){
	printf("cd\n");
      }else if(strncmp(buffer, "cd ..", 5) == 0){
	printf("cd ..\n");
      }else if(strcmp(buffer, "ls") == 0){
	printf("ls\n");
      }else if(strncmp(buffer, "mkdir", 5) == 0){
	printf("mkdir\n");
      }else if(strncmp(buffer, "create", 6) == 0){
	printf("create\n");
      }else if(strncmp(buffer, "append", 6) == 0){
	printf("append\n");
      }else if(strncmp(buffer, "remove", 6) == 0){
	printf("remove\n");
      }else if(strncmp(buffer, "delete", 6) == 0){
	printf("delete\n");
      }else if(strcmp(buffer, "dir") == 0){
	printf("dir\n");
      }else if(strcmp(buffer, "prfiles") == 0){
	printf("prfiles\n");
      }else if(strcmp(buffer, "prdisk") == 0){
	printf("prdisk\n");
      }
    }
  }
  
  return 0;
}
