#include "stage1.h"


int main(int argc, char const *argv[]) {
  if(argc<2){
      printf("arguments required for program" );
      abort();
  }
  //if no directory is entered read current directory's contetns
  else if(argc==2){
    command = argv[1];
    directory = ".";
    basicDir(command, directory);

  }
  else if (argc==3){
    //command is the first argument entered by the user
    command = argv[1];
    directory = argv[2];
    basicDir(command, directory);
  }
  else{
    printf("Too many arguments, for flags, run stage2\n");
    abort();
  }

}
