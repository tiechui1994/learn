#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <unistd.h>

/* 定义一个给 clone 用的栈，栈大小1M */
#define STACK_SIZE (1024 * 1024)
static char container_stack[STACK_SIZE];

char *const container_args[] = {
        (char *) "/bin/bash",
        NULL
};

/**
 * IPC全称 Inter-Process Communication, 是Unix/Linux下进程间通信的一种方式, IPC有
 * 共享内存、信号量、消息队列等方法. 所以, 为了隔离, 我们也需要把IPC给隔离开来, 这样, 只有
 * 在同一个Namespace下的进程才能相互通信.
 *
 * 如果你熟悉IPC的原理的话, IPC需要有一个全局的ID, 即然是全局的, 那么就意味着我们的Namespace
 * 需要对这个ID隔离, 不能让别的Namespace的进程看到.
 *
 * ipcmk -Q 创建一个ipc的Queue
 *
 * ipcs -q 查看ipc的Queue的信息
 *
 **/

int container_main(void *arg) {
    printf("Container - inside the container!\n");

    execv(container_args[0], container_args);
    printf("Something's wrong!\n");
    return 1;
}

int main() {
    printf("Parent - start a container!\n");
    /* 调用clone函数，其中传出一个函数，还有一个栈空间的（为什么传尾指针，因为栈是反着的） */
    int container_pid = clone(container_main, container_stack + STACK_SIZE,
                              CLONE_NEWIPC | SIGCHLD, NULL);
    /* 等待子进程结束 */
    waitpid(container_pid, NULL, 0);
    printf("Parent - container stopped!\n");
    return 0;
}
