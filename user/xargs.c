#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

#define TOKEN_SIZE 64

int getline(int fd, char* buf) {
    //read a char from fd
    int ind = 0;
    char c[1];
    while (read(fd, c, 1)) {
        if (c[0] == '\n') {
            return ind;
        }
        buf[ind] = c[0];
        ind++;
        //if char == \0, done, return
        //else write to buf, add index
    }
    return ind;
}

char *get_token(int *ind, char *buf) {
    //printf("buffer: %s; index: %d\n", buf, *ind);
    char *c = malloc(sizeof(char)*TOKEN_SIZE);
    int cind = 0; //for c
    //read buff, when reach space, return. update int
    while (buf[*ind] != ' ' && buf[*ind] != '\0' && cind < TOKEN_SIZE) {
        //printf("char: %c\n", buf[cind]);
        c[cind++] = buf[(*ind)++];
    }
    c[cind] = '\0';
    (*ind)++;
    return c;
} //done

int make_token_array(char **token_array, char *buf, int argc) {
    //char **token_array = malloc(sizeof(char*) * MAXARG);
    int buf_ind = 0;
    char *token = get_token(&buf_ind, buf);
    //printf("Token1: %s\n", token);
    while (token[0] != '\0' && buf_ind < 1024) {
        //add token to token array
        token_array[argc++] = token;
        //refresh token
        token = get_token(&buf_ind, buf);
        //printf("token: %s", token);
    }
    //return 
    return argc;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "too few arguments\n");
        exit(1);
    }

    //shift elements in argv back by one
    for (int i = 1; i < argc; i++) {
        argv[i-1] = argv[i];
    }
    argc -= 1;

    char buf[1024];
    while (getline(0, buf) > 0) {
        //append stuffs after argv
        //update argc;
        argc = make_token_array(argv, buf, argc);
        
        //testing: 
        // for (int i = 0; i < argc; i++) {
        //     printf("token: %s\n", argv[i]);
        // }

        //fork
        int cpid = fork();
        if (cpid == 0) {
            exec(argv[0], argv);
            exit(1);
        }
        wait(0);
        //child exec, parent wait
        //clear argv, zero argc, make argv again
        argc = 1;
    }

    //loop: 
    
    /*char *buf = "cat hello_world.txt hi.txt";
    int ind = 0;
    char *c = get_token(&ind, buf);
    printf("%s;\n", c);
    c = get_token(&ind, buf);
    printf("%s;\n", c);*/ //testing get_token
    /*char buf[32];
    int len = getline(0, buf);
    printf("%s: len = %d;\n", buf, len);*/ //testing get_line
    /*char **token_array = malloc(sizeof(char*) * MAXARG);
    char *buf = "cat hello_world.txt hi.txt";
    int token_num = make_token_array(token_array, buf, 0);
    printf("number of tokens: %d;\n", token_num);
    for (int i = 0; i < token_num; i++) {
        printf("token %d: %s\n", i+1, token_array[i]);
    }*/

    //char **token_array = malloc(sizeof(char*) * MAXARG);
    //make_token_array(token_array, buf);
    //printf("%s has %d tokens\n", buf, num);

    exit(0);
}