#include "unp.h"

void connect_alarm(int);

int connect_timeo(int sockfd, const struct sockaddr* addr, socklen_t addrlen, int nsec) {
    Sigfuc* sigfuc;
    int n;


    // 使用了系统调用 (connect) 的可中断能力, 使得它们能在内核超时时发生之前返回.
    sigfuc = Signal(SIGALRM, connect_alarm);
    if (alarm((uint) nsec) != 0) {
        err_msg("connect_timeo: alarn was already set");
    }

    if ((n = connect(sockfd, addr, addrlen)) < 0) {
        err_msg("connect error:%s, %d", strerror(errno), errno);
        Close(sockfd);
        if (errno == EINTR) {
            errno = ETIMEDOUT;
        }
    }

    alarm(0); // turn off alarm
    Signal(SIGALRM, sigfuc); // restore previous signal hanlder

    return n;
}

void connect_alarm(int signo) {
    err_msg("Received SIGALRM: %d", signo);
    return;
}

int main(int argc, char** argv) {
    int sockfd;
    struct sockaddr_in servaddr;
    int off;

    if (argc != 2) {
        err_quit("usage: tcpcli <IP>");
    }

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    Fcntl(sockfd, F_SETOWN, getpid());
    connect_timeo(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr), 1);
}
