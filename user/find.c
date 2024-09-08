#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  *(buf + strlen(p)) = '\0';
  //printf("The formatted name is: %s.\n", buf);
  return buf;
} //done

void
find(char *path, char *target)
{
    //printf("The target is: %s; \n", target);
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){ //fail to open
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){ //fill file statistics into &st, a struct stat instance
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE: //type is a file
    //TODO: throw an error as you cannot find a file in a file
    fprintf(2, "Enter a pathname for find(). Got a filename instead.\n");
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    } //handle path too long to fill into buffer
    strcpy(buf, path); //copy path to buf
    p = buf+strlen(buf); //ptr move to end of buf
    *p++ = '/'; //end of buf becomes /

    //TODO: when reading an element in dir, check if is a file
    //if a file, then check if name match
    //if name match, print file path
    //if a dir, then call find() again
    while(read(fd, &de, sizeof(de)) == sizeof(de)){ //dirent
      if(de.inum == 0) //
        continue;
      memmove(p, de.name, DIRSIZ); 
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      switch(st.type) {
        case T_FILE:
            //printf("The file name is: %s; The target is: %s\n", fmtname(buf), target);
            if (strcmp(fmtname(buf), target) == 0) {
                printf("%s\n", buf);
            }
            break;
        case T_DIR:
            //find(buf, target);
            //printf("The directory name is: %s \n", buf);
            if (strcmp(fmtname(buf), ".") != 0 && strcmp(fmtname(buf), "..") != 0) {
                find(buf, target);
            }
            break;
      }
      //printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{

  if(argc < 2) {
    fprintf(2, "too few arguments.\n");
    exit(1);
  }

  if(argc < 3){
    find(".", argv[1]);
    exit(0);
  }
  find(argv[1], argv[2]);
  exit(0);
}