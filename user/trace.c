#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//task: trace syscall
//trace(1<<SYS_fork)



int
main(int argc, char *argv[])
{
  int i;
  char *nargv[MAXARG];
  //when argument not enough
  if(argc < 3 || (argv[1][0] < '0' || argv[1][0] > '9')){
    fprintf(2, "Usage: %s mask command\n", argv[0]);
    exit(1);
  }
  //when mask illegal (not an int)
  if (trace(atoi(argv[1])) < 0) { //we learn here that trace is int trace(int)
    fprintf(2, "%s: trace failed\n", argv[0]);
    exit(1);
  }
  //produce a new arg array
  for(i = 2; i < argc && i < MAXARG; i++){
    nargv[i-2] = argv[i];
  }
  exec(nargv[0], nargv);
  exit(0);
}
