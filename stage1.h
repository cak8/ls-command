#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


//the number of contents returned by stddir
int contents;

char* directory;// The directory to read from
char* command; // the ls command entered by user

// filter for stddir
int filter(const struct dirent *entry);


//string array that stores all the contents of the directory
struct dirent** namelist;

// if the first character in of the object is a "." then don't return it
int filter(const struct dirent *entry){
   return entry->d_name[0]!='.';
}

int abcSort(const struct dirent **first, const struct dirent **second){
      return strcasecmp((*first)->d_name, (*second)->d_name);
}

//function signiture for basic directory printing
void basicDir(char* command, char* directory){
  // if the command entered is indeed ls
    if(strcmp(command,"ls")==0){
       // directory entry struct that contains a pointer to a string...

       // nr of contents returned by user
       contents=scandir(directory, &namelist,filter,abcSort);

         //if -1 is returned due to error
         if(contents<0){
           fprintf(stderr, "Error caused by input : %s\n", strerror(errno));
         }

         // if no error occured
         else{
           for (int i = 0; i < contents; i++) {
             // print name of content to stdout
             printf("%s\n",namelist[i]->d_name);
           }  
         }
	free(namelist);// needs to be freed as space is malloced for it
     }

     //if ls is not entered as first argument
     else{
     printf("Wrong command entered\n" );
     }
  }
