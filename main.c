#define _GNU_SOURCE

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
char* curDir;

/*
 * Read in the directories from the fileName
 *
 * Param fileName  the file to read
 */
void readDirs(char* fileName){
  
  FILE* fp;
  char str[2048];
  char newStr[2048];
  newStr[0] = '\0';
  fp = fopen(fileName, "r");

  /*read from file */
  while(fgets(str, 2048, fp) != NULL){
    if(strlen(str) > 1){/* ensure no blank lines at end of file */  
      sscanf(str, "%s", newStr);
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
 
  /* read from file */
  while(fgets(str, 2048, fp) != NULL){
    if(strlen(str) > 1){ /*ensure no blank lines at end of file */
      sscanf(str, "%ld %ld %s %ld %s %s %ld %s %s %s %[^\n]", &fstNum, &sndNum, perm, &thdNum, nameOne, nameTwo, &fileSize, timeStampM, timeStampN, timeStampTwo, fName); /*format input string into temp */
 
      char* timeStampF = (char*)malloc(sizeof(char) * 35);
      timeStampF[0] = '\0';
      strcat(timeStampF, timeStampM);
      strcat(timeStampF, " ");
      strcat(timeStampF, timeStampN);
      strcat(timeStampF, " ");
      strcat(timeStampF, timeStampTwo);
     
      allocate(addFullFileFromRoot(fileSystem, fName, fileSize, timeStampF), lDiskList, 1);
    }
  }
  fclose(fp);
}

/*print out the current directory path*/
void printCur(char* curDir){
  printf("FILESYSTEM: %s $: ", curDir);
}

/*exit the simulation*/
void exitFileSystem(){
  //cleanup
  printf("Leaving FILESYSTEM...\n");
  exit(0);
}

/*change the directory*/
void cd(char* path){
  char* tempOldPath = (char*) malloc(sizeof(char)*strlen(curDir)+1);
  strcpy(tempOldPath, curDir);
  int numNames = countOccurance(path, '/') + 1;
  char** names = (char**) malloc(sizeof(char*)*numNames);
  char* tempName = (char*) malloc(sizeof(char)*strlen(path)+1);
  strcpy(tempName, path);
  if(numNames > 1){
    int counter = 0;
    int i = 0;
    
    char* s = strtok(tempName, "/");
    while(s!=NULL){
      counter++;
      names[i++] = s;
      s=strtok(NULL, "/");
      
    }
    
  }else{
    names[0] = path;
  }
  
  for(int i = 0; i< numNames; i++){
    if(strcmp("..", names[i]) == 0){
      
      if(strcmp(".", curDir) == 0){
	printf("Invalid Directory\n");
	strcpy(curDir, tempOldPath);
	free(tempOldPath);
	return;
      }else{
	char* newDir = strrchr(curDir, '/');
	int loc = (int) (newDir-curDir);
	char* path = malloc(sizeof(char)*(loc+1));
	strncpy(path, curDir, loc);
	path[loc] = '\0';
	strcpy(curDir, path);
	
	free(path);
      }
    }else{
      strcat(curDir, "/");
      strcat(curDir, names[i]);
    }
  }
  if(!pathExist(fileSystem,curDir)){
    printf("Invalid Directory\n");
    strcpy(curDir, tempOldPath);
    free(tempOldPath);
  }
  free(tempName);
  
}
 
int main(int argc, char *argv[]){
  /*variables*/
  char* buffer;
  curDir = (char*)malloc(2048);
  curDir[0] = '\0';
  buffer = (char*)malloc(2048);
  size_t sizetoread = 2048;
  /*extract args and error check */
  if(argc < 5){
    perror("Must provide 4 arguments");
    exit(1);
  }
  
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
  
  
  readDirs(argv[2]);
 
  readFile(argv[1]);
  
  curDir = ((TreeNode)(parseTree(fileSystem, "")))->dir;
  
  //handle inputs 
  while(1){
    printCur(curDir);
    int n = getline(&buffer, &sizetoread, stdin);
    buffer[n-1] = '\0';
    
    if(strcmp(buffer, "exit") == 0){
      exitFileSystem();
    }else if(strncmp(buffer, "cd ", 3) == 0){
      cd(buffer+3);
    }else if(strcmp(buffer, "ls") == 0 && strlen(buffer)==2){
      printTnode(((TreeNode)parseTree(fileSystem, curDir))->children);
    }else if(strncmp(buffer, "mkdir", 5) == 0){
      /*call addDirFromRoot. This is in Tree.c The tree param is the fileSystem global variable
       * The fullName param is the current directory (curDir) concatinated with the new directory to be added
       * Before you call addDirFromRoot, call pathExist to check if the directory already exists
       */

      char* temp = (char*)malloc(2048);
      strcpy(temp, curDir);
      strcat(temp, "/");
      strcat(temp, buffer + 6);
      if(pathExist(fileSystem, temp)){
	printf("directory already exists\n");
      }else{
	addDirFromRoot(fileSystem, temp);
      }
      free(temp);
      temp = NULL;
      
    }else if(strncmp(buffer, "create", 6) == 0){
      /*Call allocate. This is in FileMemory.c . The first param is gotten from calling 
	addFileFromRoot in Tree.c. Pass NULL as the timestamp param, and get size param from user input
	The LL param is our lDiskList global variable, and the load param is 0 */
      long sizeN;
      char nameN[1024];
      sscanf(buffer+7, "%s %ld", nameN, &sizeN);
      TreeNode treeNodeN = addFileFromRoot(fileSystem, curDir, nameN, sizeN, NULL);
      if(treeNodeN == NULL){
	/*null return*/
      }else{
	allocate(treeNodeN, lDiskList, 0);
      }
    }else if(strncmp(buffer, "append", 6) == 0){
      /* Call expand. This is in FileMemory.c 
       * Get the first param by calling parseTree in tree.c, with fullname being the concatinated curDir and filename
       * LL is lDiskList
       * size is amount to add 
       */
      long sizeN;
      char nameN[1024];
      sscanf(buffer+7, "%s %ld", nameN, &sizeN);
	 
      char* temp = (char*)malloc(2048);
      strcpy(temp, curDir);
      strcat(temp, "/");
      strcat(temp, nameN);
      TreeNode td = parseTree(fileSystem, temp); 
      if(td != NULL && enoughMemory(lDiskList, sizeN)){
		expand(td, lDiskList, sizeN);
      }
      free(temp);
      temp = NULL;
    }else if(strncmp(buffer, "remove", 6) == 0){
      /*same as append, but call shink*/
      long sizeN;
      char nameN[1024];
      sscanf(buffer+7, "%s %ld", nameN, &sizeN);
      char* temp = (char*)malloc(2048);
      strcpy(temp, curDir);
      strcat(temp, "/");
      strcat(temp, nameN);
      TreeNode td = parseTree(fileSystem, temp); 
      if(td != NULL){
		shrink(td, lDiskList, sizeN);
	  }
      free(temp);
      temp = NULL;
    }else if(strncmp(buffer, "delete", 6) == 0){
      char* temp = (char*)malloc(2048);
      strcpy(temp, curDir);
      strcat(temp, "/");
      strcat(temp, buffer + 7);
      TreeNode td = parseTree(fileSystem, temp); 
      if(td != NULL){
		shrink(td, lDiskList, td->size);
		TreeNode tdTwo = deleteFromRoot(fileSystem, curDir, buffer + 7);
		freeTreeNode(tdTwo);
      }
      free(temp);
      temp = NULL;
    }else if(strcmp(buffer, "dir") == 0){
		printBFS(fileSystem);
    }else if(strcmp(buffer, "prfiles") == 0){
      printf("prfiles\n");
    }else if(strcmp(buffer, "prdisk") == 0){
		
      printf("prdisk\n");
    }
    
  }
  
  return 0;
}
