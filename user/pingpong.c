#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    char c = 42;
    char rec;
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);
    int cpid = fork();
    if (cpid == 0) {
        //child
        close(p1[1]);
        close(p2[0]);
        read(p1[0], &rec, 1);
        cpid = getpid();
        printf("%d: received ping\n", cpid);
        write(p2[1], &rec, 1);
        close(p1[0]);
        close(p2[1]);
        exit(0);
    } else {
        //parent
        close(p1[0]);
        close(p2[1]);
        write(p1[1], &c, 1);
        read(p2[0], &rec, 1);
        cpid = getpid();
        printf("%d: received pong\n", cpid);
        close(p1[1]);
        close(p2[0]);
        exit(0);
    }
}