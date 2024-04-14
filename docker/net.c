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

/** 新的NET协议栈, 里面是没有网络的 **/
int container_main(void *arg) {
    printf("Container - inside the container!\n");
    printf("received arg: %s \n", (char *) arg);
    system("ping www.baidu.com -t 4");
    execv(container_args[0], container_args);
    printf("Something's wrong!\n");
    return 1;
}

int main() {
    printf("Parent - start a container!\n");
    /**
     * 调用clone函数, 其中传出一个函数, 还有一个栈空间的(为什么传尾指针, 因为栈是反着的)
     * clone (int (*fn) (void* arg), void* child_stack, int flags, void *arg, ...)
     *
     * 第一个参数是函数指针(返回值是int, 参数是void*)
     * 第二个参数是栈指针
     * 第三个参数是namespace
     * 第四个参数是函数的参数
     *
     * int unshare (int flags)  取消 namespace
     *
     * int setns (int fd, int nstype) 设置namespace
     **/

    int container_pid = clone(container_main, container_stack + STACK_SIZE,
                              CLONE_NEWNET | SIGCHLD, (void *) "this function params");
    /* 等待子进程结束 */
    waitpid(container_pid, NULL, 0);
    printf("Parent - container stopped!\n");
    return 0;
}
