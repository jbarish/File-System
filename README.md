# File System Simulation
This program is an interactive simulation of a file system.    
It uses a tree structure to represent the directory hierachary, a linked list of disk space, and a linked-list of data blocks.   
   
The program is run with 4 arguments:   
   P1) The text file containing the input files to be loaded in
   P2) The text file containing the directory list to be loaded in
   P3) The size of the disk
   P4) The size of each data block   
   
   
The two input files are generated as follows:   
   dir_list.txt:   
   > find ./ -type d > dir_list.txt   
   file_list.txt:    
   >find ./type f -ls > file_list.txt    
       
While running the program, the user can interact with the file system with the following commands:   
   cd [directory]          change directory to the one specified (can use ..)    
   ls                      list all files and sub-directories in the current directory   
   mkdir [name]            create a new directory in the current directory    
   create [name]           create a new file in the current directory   
   aooend [name] [bytes]   add the number of bytes to a specific file   
   remove [name] [bytes]   remove the number of bytes from a specific file   
   delete [name]           delete the specified file   
        
   exit     exit the program
   dir      print out the directory tree in breadth-first order   
   prfiles  print out all file information (including the disk blocks allocated to each file   
   prdisk   print out the disk space information   
   
      
  Sample execution of the program:   
  > ./File file_list.txt dir_list.txt 1000000000 1024
  
  

