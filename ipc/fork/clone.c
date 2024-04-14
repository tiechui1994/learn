#define _GNU_SOURCE

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <string.h>

#define STACK_SIZE 1024*4

#define errEixt(msg)        \
    do {                    \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while(0)

int fn(void* args) {
    struct utsname uts;

    if (sethostname(args, strlen(args)) == -1) {
        errEixt("sethostname");
    }

    if (uname(&uts) == -1) {
        errEixt("uname");
    }

    printf("uts.nodename in child: %s\n", uts.nodename);
    sleep(10);
    return 0;
}

int main(int argc, char** argv) {
    char* stk;
    int pid;
    struct utsname uts;

    stk = malloc(STACK_SIZE);
    if (stk == NULL) {
        errEixt("malloc");
    }

    pid = clone(fn, stk + STACK_SIZE, CLONE_IO | SIGCHLD, "ooxx");
    if (pid == 0) {
        printf("child\n");
    } else if (pid > 0) {
        printf("parent, pid: %d\n", pid);
    } else {
        errEixt("clone");
    }

    sleep(1);

    if (uname(&uts) == -1) {
        errEixt("uname");
    }

    printf("uts.nodename in parent: %s\n", uts.nodename);

    if (waitpid(pid, NULL, 0) == -1) {
        errEixt("waitpid");
    }
}