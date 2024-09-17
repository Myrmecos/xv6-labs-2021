#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char ch='Q';
  write(1,&ch,sizeof(ch));
  exit(0);
}