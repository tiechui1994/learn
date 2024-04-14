#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <unistd.h>

static jmp_buf env;


static void handle(int signum, siginfo_t* info, void* secret) {
    printf("crash signum:%d si_code:%d\n", signum, info->si_code);
    siglongjmp(env, 1);
}

static void sigsetup2(void) {
    struct sigaction act;
    memset(&act, 0, sizeof act);
    act.sa_flags = SA_ONSTACK | SA_SIGINFO;
    act.sa_sigaction = handle;
    sigaction(SIGSEGV, &act, 0);
    sigaction(SIGABRT, &act, 0);
}

typedef void (* cb)(int*);

static void mysetjmp(cb f, int* arg) {
    bzero(env, sizeof(env));
    int r = sigsetjmp(env, 1);
    if (r == 0) {
        f(arg);
    } else {
        printf("异常后恢复\n");
    }
    printf("++++++++++++++\n\n");
    sleep(2);
}

void test_crash2(int* a) {
    int i, sum = 0, num = (int) (random() % 10000);

    for (i = 1; i < num; i++) {
        sum += i;
    }
    printf("i: %d, sum: %d %d ", i, sum, a == NULL);
    printf("crash: %d\n", *a);

    *a += 1;
}


int main(int argc, char** argv) {
    sigsetup2();
    int* a;
    int b;
    mysetjmp(test_crash2, a);
    mysetjmp(test_crash2, &b);
    mysetjmp(test_crash2, a);
    mysetjmp(test_crash2, &b);
    mysetjmp(test_crash2, a);

    printf("hahhahahahahhahaha\n");

    return 0;
}