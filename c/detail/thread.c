#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <sys/syscall.h>

#define gettid() syscall(__NR_gettid)

/**
 * 多线程: pthread.h, gcc链接时参数: -l pthread
 * int pthread_create (pthread_t *tid, const pthread_attr_t *attr,
 *              void * (*func) (void *),
 *              void * arg)
 *
 * pthread_t, 线程id, 无符号整型
 * pthread_attr_t, 线程属性, 比如线程优先级, 初始栈大小等
 * void * (*func)(void *arg), 线程执行函数指针
 * void *, 参数
 *
 * union pthread_attr_t {
 *   char size[56];
 *   long int align;
 * };
 *
 * void pthread_exit(void *status), 退出线程
 * status: 保存退出状态
 *
 * int pthread_join(pthread_t tid, void **status), 等待线程退出
 * tid, 指定等待的线程id
 * status, 保存退出值
 *
 *
 * pthread_t pthread_self(void)  获取当前线程id
 *
 * int pthread_detach(pthread_t tid) 分离线程
 * 变为分离状态的线程, 如果线程退出, 他的所有资源全部释放; 而如果不是分离状态, 线程必须保留它的线程id, 退出状态直到其他线程对它调用pthread_join
 *
 **/

/**
 * 互斥锁:
 *
 * static pthread_mutext_t mutex = PTHREAD_MUTEX_INITIALIZER // 静态初始化
 * int pthread_mutext_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr) // 动态初始化
 * mutext: 互斥变量
 * attr: 锁属性, NULL值表示默认属性
 *
 * 加锁:
 * int pthread_mutex_lock(pthread_mutex_t *mutex) // 阻塞方式
 *
 * int pthread_mutex_trylock(pthread_mutex_t *mutex) // 非阻塞
 *
 * int pthread_mutex_unlock(pthread_mutex_t *mutex) // 释放锁
 *
 * int pthread_mutex_destroy(pthread_mutex_t *mutex) // 销毁
 *
 *
 *
 * 条件变量: 条件变量是用来等待而不是用来上锁的, 条件变量用来自动阻塞一个线程, 直到特殊情况发生为止; 通常条件变量和互斥锁同时使用.
 *
 * 条件变量使我们可以睡眠等待某种条件出现; 条件变量是利用线程间共享全局变量进行同步的一种机制, 主要包含两个动作: 一个线程等待"条件变量的条件成立"
 * 而挂起; 另一个线程使"条件成立".
 *
 * 条件的检测是在互斥锁的保护下进行的; 如果一个条件为假, 一个线程自动阻塞, 并释放等待状态改变的互斥锁;
 * 如果另一个线程改变了条件, 它发信号给关联的条件变量, 唤醒一个或多个等待它的线程, 重新获得互斥锁, 重新评价条件;
 *
 * 如果两进程共享可读写的内存, 条件变量可以被用来实现这两进程间的线程同步;
 *
 * static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;                      // 静态初始化
 * int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *cond_attr); // 动态初始化
 * int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);        // 等待条件, 阻塞
 * int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex *mutex, const timespec *abstime); // 等待条件,超时
 * int pthread_cond_signal(pthread_cond_t *cond);     // 通知条件,只唤醒单个等待线程
 * int pthread_cond_broadcast(pthread_cond_t *cond);  // 通知条件,唤醒所有等待线程
 * int pthread_cond_destroy(pthread_cond_t *cond);    // 销毁
 *
 *
 * 信号量: 锁机制使用是有限的, 锁只有两种状态, 即加锁和解锁, 对于互斥的访问一个全局变量, 这种方式还可以对付, 但是要是对于其他的临界资源, 比如
 * 多台打印机等, 这种方式显然不行了.
 * 信号量是一个整数计数器, 其数值表示空闲临界资源的数量.
 *
 * typedef union {
 *  char size[32];
 *  long int align;
 * } sem_t
 *
 * int sem_init(sem_t *sem, int pshared, unsigned int value) // 初始化信号量
 * pshared, 非0的时候表示该信号量在进程间共享, 否则只能在当前进程的所有线程间共享.
 * value, 信号量的初始值.
 *
 * int sem_wait(sem_t *sem)  // 信号量减1操作, 有线程申请资源.
 *
 * int sem_post(sem_t *sem)  // 信号量加1操作, 有线程释放资源
 *
 * int sem_destroy(sem_t *sem) // 销毁信号量
 *
 */

/**
 * 互斥锁 + 信号量通常使用:
 *  lock -> wait -> unlock
 *  lock -> signal(条件) -> unlock
 */
static volatile int flag;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* job1() {
    printf("++++++++++ into thread 1 (pid:%d, tid:%lu) ++++++++++++++\n", getpid(), gettid());
    sleep(2);

    errno = pthread_mutex_lock(&mutex);
    if errno {
        perror("pthread_mutex_lock");
        exit(EXIT_FAILURE);
    }

    printf("<thread 1> before:%d\n", flag);
    if (flag == 2) {
        errno = pthread_cond_signal(&cond);
        if errno {
            perror("pthread_cond_signal");
            exit(EXIT_FAILURE);
        }
    }
    flag = 1;
    printf("<thread 1> after:%d\n", flag);

    errno = pthread_mutex_unlock(&mutex);
    if errno {
        perror("pthread_mutex_unlock");
        exit(EXIT_FAILURE);
    }

    printf("++++++++++ leave thread 1 (pid:%d, tid:%lu) ++++++++++++++\n", getpid(), gettid());
    return NULL;
}

void* job2() {
    printf("++++++++++ into thread 2 (pid:%d, tid:%lu) ++++++++++++++\n", getpid(), gettid());
    sleep(2);

    errno = pthread_mutex_lock(&mutex);
    if errno {
        perror("pthread_mutex_lock");
        exit(EXIT_FAILURE);
    }

    printf("<thread 2> before:%d\n", flag);
    if (flag == 1) {
        errno = pthread_cond_signal(&cond);
        if errno {
            perror("pthread_cond_signal");
            exit(EXIT_FAILURE);
        }
    }
    flag = 2;
    printf("<thread 2> after:%d\n", flag);

    errno = pthread_mutex_unlock(&mutex);
    if errno {
        perror("pthread_mutex_unlock");
        exit(EXIT_FAILURE);
    }

    printf("++++++++++ leave thread 2 (pid:%d, tid:%lu) ++++++++++++++\n", getpid(), gettid());
    return NULL;
}

void thread_and_cond() {
    pthread_t tid1, tid2;

    printf("++++++++++ into main (pid:%d, tid:%lu) ++++++++++++++\n", getpid(), gettid());
    errno = pthread_create(&tid1, NULL, (void*) &job1, NULL);
    if errno {
        perror("create thread 1 error");
        exit(EXIT_FAILURE);
    }

    errno = pthread_create(&tid2, NULL, (void*) &job2, NULL);
    if errno {
        perror("create thread 2 error");
        exit(EXIT_FAILURE);
    }

    printf("main wait flag: 1->2 or 2->1\n");
    errno = pthread_mutex_lock(&mutex);
    if errno {
        perror("pthread_mutex_lock");
        exit(EXIT_FAILURE);
    }

    errno = pthread_cond_wait(&cond, &mutex);
    if errno {
        perror("pthread_cond_wait");
        exit(EXIT_FAILURE);
    }

    errno = pthread_mutex_unlock(&mutex);
    if errno {
        perror("pthread_mutex_unlock");
        exit(EXIT_FAILURE);
    }


    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);

    printf("++++++++++ leave main (pid:%d, tid:%lu) ++++++++++++++\n", getpid(), gettid());
}

int main() {
    thread_and_cond();
}