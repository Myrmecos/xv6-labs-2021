#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/sysinfo.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 1) {
        fprintf(2, "%s: usage: %s\n");
        exit(1);
    }

    struct sysinfo info;
    sysinfo(&info);
    printf("Free space: %d\n; nused process: %d\n", info.freemem, info.nproc);
    exit(0);
}