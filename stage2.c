#include "stage2.h"
#include "stage1.h"
#include <stdbool.h>
#include <stdint.h>
#include <pwd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

bool iflag = false;
bool nflag = false;
bool lflag = false;
bool sflag = false;


int sizesort(const struct dirent** first, const struct dirent** second);

int main(int argc, char const *argv[]) {
  char* directory;

// if there are only 3 arguments and none of them are flags, set the last argument as directory and the first one( after the executable name) to the command
  
  if((argc==3)&&(strstr(argv[2], "-")==NULL)) {
    directory = argv[2];
    basicDir(argv[1], directory);
  }


  else{
    path = composePath(path, argc, argv);//compose path for individual directory contents
    // fill an array with all contents
    storeFlags(argc, argv);//read in the flags turning valid booleans true
    int contents;// the variable that carries the amount entries in a directory

    if(sflag){
      contents=scandir(path, &namelist,filter,sizesort);// if sorting is required, sort the entries according to size
    }
    else{
      contents=scandir(path, &namelist,filter,abcSort);// else sort alphabetically
    }
    storeInfoStruct(contents, argv);// load in array of stat structs
    printflags(contents);// print the content required due to valid flags

  }
  free(namelist);// free the object as it has been malloced in scandir
  printf("DONE\n" );
}


int sizesort(const struct dirent** first, const struct dirent** second){

    char temp[255];
    strcpy(temp, path);
    char* subPath = strcat(temp, "/");


    char firstPath[255];
    strcpy(firstPath, subPath);
    strcat(firstPath,(*first)->d_name);

    struct stat firstStat;
    int fir =  lstat(firstPath, &firstStat);


    char secondPath [255];
    strcpy(secondPath, subPath);
    strcat(secondPath,(*second)->d_name);

    struct stat secondStat;
    int sec =  lstat(secondPath, &secondStat);



   if(fir!=-1 && sec != -1){
    return (firstStat.st_size<=secondStat.st_size)? 1 : -1;
   }
   else{
     printf("Error occured");
     abort();
   }

}

 char* composePath(char* path, int argc , char const *argv[]){
  // loop through argv to find and distinguish path from flags
    for(int i = 2; i<argc; i++){
      if(strstr(argv[i],"-")==NULL){
        path=argv[i];
        pathIndex=i;
        break;
      }
    }
    //if no path is found check current directory
    if(strcmp(path,"")==0){
      path = ".";
    }
      return path;
  }




  void storeInfoStruct(int contents, const char* argv[]){
    //malloc for content array of buffer structs
    //struct stat*

    contArray = malloc(contents * sizeof(struct stat ));
      for (int i = 0; i < contents; i++) {
        // create path by concatinating path and nameslist->.d_name
        char temp[500];
        if(pathIndex!=-1){
          strcpy(temp, argv[pathIndex]);

        }
        else{
          strcpy(temp ,path);
        }

        //create path to individual content by concatinating filename to directory path, given in the arguments
        strcat(temp, "/");
        char* compPath = strcat(temp, namelist[i]->d_name);

        //loading content info into stat struct
        int status = lstat(compPath, &contArray[i]);// not sure about syntax
        if(status == -1){
          //read in directory contents and various metadate to the buffer
          fprintf(stderr, "Error caused in buffer due to flags : %s\n", strerror( errno));
          abort();
        }
      }
    }


    void storeFlags(int argc, const char* argv[]){
      //now find all the flag in the args list by starting at argv[2]  by looping
      // arv[0]=execuable name and argv[1]=ls
      for (int i = 2; i < argc; i++) {
        if(pathIndex==i){
          continue;
        }
        else{
            if(strcasecmp(argv[i], "-i")==0){
              iflag=true;
            }
            else if(strcasecmp(argv[i], "-n")==0){
              nflag=true;
            }
            else if(strcasecmp(argv[i], "-l")==0){
              lflag=true;
            }
            else if(strcasecmp(argv[i], "-S")==0){
              sflag=true;
            }
        }
      }
    }

    void printflags(int contents){
      //else store flag in flags array where
      for(int i =0; i<contents; i++){
        printf("-----------------------------------------------\n" );
          if(iflag){
            addInode(&(contArray[i]));
          }
          if(nflag||lflag){
            addInfo(&(contArray[i]));
          }
            printf("filename: %s\n\n" ,namelist[i]->d_name);
      }
    }

   void addInode(struct stat *buffstr ){
      printf("%lu \t",buffstr->st_ino);
   }

  void addInfo(struct stat *buffstr ){
      ///*************Permissions**************************
     printf((S_ISDIR(buffstr->st_mode) ? "d" : "-")); // is it a directory
     printf((buffstr->st_mode & S_IRUSR) ? "r" : "-");// is it readable by user
     printf((buffstr->st_mode & S_IWUSR) ? "w" : "-");// is it writeable//editable by user
     printf((buffstr->st_mode & S_IXUSR) ? "x" : "-");// is it executable by user
     printf((buffstr->st_mode & S_IRGRP) ? "r" : "-");// is it  readable by group
     printf((buffstr->st_mode & S_IWGRP) ?  "w" : "-");// is it editable by group
     printf((buffstr->st_mode & S_IXGRP) ? "x" : "-");// is it executable by group
     printf((buffstr->st_mode & S_IROTH) ? "r" : "-");// is it readable by others
     printf ((buffstr->st_mode & S_IWOTH) ? "w" : "-");// is it editable by others
     printf((buffstr->st_mode & S_IXOTH) ? "x" : "-");// is it executable by others
     printf("\t");


     ////**************Size in bytes*******************
     printf("%ld bytes\t", buffstr->st_size);

     ////modification time
     struct tm* timeinfo;
     time_t t  = buffstr->st_mtim.tv_sec;
     timeinfo = localtime (&t);
     printf("%s",asctime(timeinfo) );


     //id or name
     if(nflag==true){addId(buffstr);}
      else{ addName(buffstr);}
    }

    void addId(struct stat *buffstr ){
      printf("userId:%u\tgroupId:%u\t", buffstr-> st_uid, buffstr-> st_gid);
    }

    void addName( struct stat *buffstr){
        struct passwd *userStr = getpwuid(buffstr->st_uid);
        char* username = userStr->pw_name;

        struct group* grpStr = getgrgid(buffstr->st_gid);
        char* groupname = grpStr->gr_name;

      printf("username:%s\tgroupname:%s\t", username, groupname);
    }
