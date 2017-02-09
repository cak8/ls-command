#include <sys/types.h>
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>



// struct that stored info about a certain file
struct stat buffer;

// buffer array
struct stat* contArray ;


char* path ="";
int pathIndex = -1;




void addInode(struct stat* buffstr);
void addId(struct stat* buffstr);
void storeInfoStruct(int contents, const char* argv[]);
void storeFlags(int argc, const char* argv[]);
void printflags(int contents);
void addInfo( struct stat *buffstr );
void addName( struct stat *buffstr);
char* composePath(char* path, int argc , char const *argv[]);
