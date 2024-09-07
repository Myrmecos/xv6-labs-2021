#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;
  // argv is an array of char array
  for(i = 1; i < argc; i++){
    //start from 2nd token, 
    //write the ith token to standard output
    write(1, argv[i], strlen(argv[i]));
    //add \0 terminator at the end of output
    if(i + 1 < argc){
      write(1, " ", 1);
    } else {
      write(1, "\n", 1);
    }
  }
  exit(0);
}
