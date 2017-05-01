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
  char* buffer;
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
  char timestampM[7];
  char timestampN[5];
  char timestampTwo[13];
  char fName[1024];
  fp = fopen(fileName, "r");
 
  /* read from file */
  while(fgets(str, 2048, fp) != NULL){
    if(strlen(str) > 1){ /*ensure no blank lines at end of file */
      sscanf(str, "%ld %ld %s %ld %s %s %ld %s %s %s %[^\n]", &fstNum, &sndNum, perm, &thdNum, nameOne, nameTwo, &fileSize, timestampM, timestampN, timestampTwo, fName); /*format input string into temp */
 
      char* timestampF = (char*)malloc(sizeof(char) * 35);
      timestampF[0] = '\0';
      strcat(timestampF, timestampM);
      strcat(timestampF, " ");
      strcat(timestampF, timestampN);
      strcat(timestampF, " ");
      strcat(timestampF, timestampTwo);
     
      allocate(addFullFileFromRoot(fileSystem, fName, fileSize, timestampF), lDiskList, 1);
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
  disposeTree(fileSystem->root);
  while(getNumElements(lDiskList)>0){
	  free(removeAt(lDiskList,0));
  }
  free(lDiskList);
  
  free(fileSystem);
  free(buffer);
  free(curDir);
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
    numNames = counter;
    
  }else{
    names[0] = path;
  }
  
  for(int i = 0; i< numNames; i++){
    if(strcmp("..", names[i]) == 0){
      
      if(strcmp(".", curDir) == 0){
	printf("Invalid Directory\n");
	strcpy(curDir, tempOldPath);
	free(tempOldPath);
	 free(tempName);
  free(names);
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
  int pe = pathExist(fileSystem,curDir);
  if(pe == 0 || pe == 2){
    printf("Invalid Directory");
	if(pe){
		printf(" Cannot cd into a file");
	}
	printf("\n");
    strcpy(curDir, tempOldPath);
    
  }
  free(tempOldPath);
  free(tempName);
  free(names);
}
 
int main(int argc, char *argv[]){
  /*variables*/

  curDir = (char*)malloc(sizeof(char)*2048);
  curDir[0] = '\0';
  buffer = (char*)malloc(sizeof(char)*2048);
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
  
  strcpy(curDir ,((TreeNode)(parseTree(fileSystem, "")))->dir);
  
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
	  if(strlen(buffer + 7) > 0){
	    TreeNode treeNodeN = addFileFromRoot(fileSystem, curDir, buffer + 7, 0, getTimeAsString());
		TreeNode d = getParent(fileSystem, curDir);
		d->timestamp = getTimeAsString();
	  }else{
		 printf("Must provide filename\n");
	 }
    }else if(strncmp(buffer, "append", 6) == 0){
      /* Call expand. This is in FileMemory.c 
       * Get the first param by calling parseTree in tree.c, with fullname being the concatinated curDir and filename
       * LL is lDiskList
       * size is amount to add 
       */
      long sizeN=0;
      char nameN[1024] = "";
      sscanf(buffer+7, "%s %ld", nameN, &sizeN);
      if(strlen(nameN)>0){
		if(sizeN < 0){
		  printf("Size cannot be less than 0\n");
		}else{
		  char* temp = (char*)malloc(2048);
		  strcpy(temp, curDir);
		  strcat(temp, "/");
		  strcat(temp, nameN);
		  TreeNode td = parseTree(fileSystem, temp); 
		  
		  if(td != NULL && enoughMemory(lDiskList, sizeN) && td->dir ==NULL){
			expand(td, lDiskList, sizeN);
			TreeNode d = getParent(fileSystem, curDir);
			d->timestamp = getTimeAsString();
		  }else{
			printf("Must specify a filename only\n");
		  }
		  free(temp);
		  temp = NULL;
		}
      }else{
	printf("Must provide filename\n");
      }
    }else if(strncmp(buffer, "remove", 6) == 0){
      /*same as append, but call shink*/
      long sizeN=0;
      char nameN[1024]="";
      sscanf(buffer+7, "%s %ld", nameN, &sizeN);
      if(strlen(nameN)>0){
	if(sizeN < 0){
	  printf("Size cannot be less than 0\n");
	}else{
	  char* temp = (char*)malloc(2048);
	  strcpy(temp, curDir);
	  strcat(temp, "/");
	  strcat(temp, nameN);
	  TreeNode td = parseTree(fileSystem, temp); 
	  if(td != NULL && td->dir ==NULL){
	    shrink(td, lDiskList, sizeN);
		TreeNode d = getParent(fileSystem, curDir);
		d->timestamp = getTimeAsString();
	  }else{
	    printf("Must specify a filename only\n");
	  }
	  free(temp);
	  temp = NULL;
	}
      }else{
	printf("Must provide filename\n");
      }
    }else if(strncmp(buffer, "delete", 6) == 0 ){
      if( strlen(buffer)>7){
	char* temp = (char*)malloc(2048);
	strcpy(temp, curDir);
	strcat(temp, "/");
      strcat(temp, buffer + 7);
      TreeNode td = parseTree(fileSystem, temp); 
      if(td != NULL){
	if(td->dir==NULL){
	  shrink(td, lDiskList, td->size);
	  TreeNode d = getParent(fileSystem, curDir);
		d->timestamp = getTimeAsString();
	}
	TreeNode tdTwo = deleteFromRoot(fileSystem, curDir, buffer + 7);
	if(tdTwo != NULL){
	  freeTreeNode(tdTwo);
	}
      }
      free(temp);
      temp = NULL;
      }else {
	printf("Must provide a filename to delete\n");
      }
    }else if(strcmp(buffer, "dir") == 0){
      printBFS(fileSystem);
    }else if(strcmp(buffer, "prfiles") == 0){
      printFiles(fileSystem);
    }else if(strcmp(buffer, "prdisk") == 0){
      printLDnode(lDiskList);
      printf("fragmentation: %ld\n", getFragmentation(fileSystem->root));
    }else if (strncmp(buffer, "cd..",4)==0){
		printf("-FILESYSTEM: invalid command. Try 'cd ..'\n");
	} else{
      printf("-FILESYSTEM: %s: command not found\n", buffer);
    }
    
  }

  return 0;
}
