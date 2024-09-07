#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[]) 
{
    int p[2];
    int p1[2];
    pipe(p);
    int pid = fork();
    while (pid == 0) {
        //child:
        //close write end to p1
        int ret; //the prime to be printed
        int cur; //the other numbers to be inspected
        close(p[1]);
        int readret = read(p[0], &ret, 4);
        //if no number, exit;
        if (readret == 0) {
            close(p[0]);
            exit(0);
        }
        //print first number, p
        printf("prime %d\n", ret);

        //make a new pipe, p1 
        pipe(p1);
        //fork the next child process
        pid = fork();

        //child:
            //shift the new pipe, p1 as p
        if (pid == 0) {
            p[0] = p1[0];
            p[1] = p1[1];
        } else { //parent, i.e. this process
            //filter the rest of number, send to next process
            while (readret != 0) {
                readret = read(p[0], &cur, 4);
                if (cur%ret) {
                    write(p1[1], &cur, 4);
                }
            }
            //close read end of p
            close(p[0]);
            //close write end of p1
            close(p1[1]);
            //wait
            wait(0);
            //exit
            exit(0);
        }
    }
    
    close(p[0]);
    for (int i = 2; i <= 35; i++) {
        write(p[1], &i, 4);
    }
    close(p[1]);
    wait(0);
    exit(0);
    //parent:
        //close read end to p1
        //write to p1
        //close write end
        //exit
}