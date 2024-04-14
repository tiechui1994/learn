#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <cstdlib>

/* 定义一个给 clone 用的栈，栈大小1M */
#define STACK_SIZE (1024 * 1024)
static char container_stack[STACK_SIZE];

char *const container_args[] = {
        (char *) "/bin/bash",
        NULL
};

/**
 * 在通过CLONE_NEWNS创建mount namespace后，父进程会把自己的文件结构复制给子进程中.
 *
 * 而子进程中新的namespace中的所有mount操作都只影响自身的文件系统, 而不对外界产生任何影响.
 *
 * 这样可以做到比较严格地隔离
 */
int container_main(void *arg) {
    printf("Container [%3d] - inside the container!\n", getpid());

    sethostname("master", 10);

    /** 启用了mount namespace并在子进程中重新mount了/proc文件系统 **/
    system("mount -t proc proc /proc");

    /** 直接执行一个shell, 以便我们观察这个进程空间里的资源是否被隔离了 **/
    execv(container_args[0], container_args);
    printf("Something's wrong!\n");
    return 1;
}

int main() {
    printf("Parent - start a container!\n");
    /* 调用clone函数，其中传出一个函数，还有一个栈空间的（为什么传尾指针，因为栈是反着的） */
    int container_pid = clone(container_main, container_stack + STACK_SIZE,
                              CLONE_NEWUTS | CLONE_NEWPID | CLONE_NEWNS | SIGCHLD, NULL);
    /* 等待子进程结束 */
    waitpid(container_pid, NULL, 0);
    printf("Parent - container stopped!\n");
    return 0;
}
