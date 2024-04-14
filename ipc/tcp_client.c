#include "unp.h"

void sig_pipe(int signo);

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

    Signal(SIGPIPE, sig_pipe);
    Fcntl(sockfd, F_SETOWN, getpid());

    off = 0;
    Setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &off, sizeof(off));

    Connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr));

    char sendline[MAXLINE], recvline[MAXLINE];
    while (Fgets(sendline, MAXLINE, stdin) != NULL) {
        Writen(sockfd, sendline, 1);
        sleep(1);
        Writen(sockfd, sendline + 1, strlen(sendline) - 1);
        printf("write success\n");

        if (Readline(sockfd, recvline, MAXLINE) == 0) {
            err_quit("server terminated permaturely");
        }

        Fputs(recvline, stdout);
    }

    exit(0);
}

void sig_pipe(int signo) {
    printf("SIGPIPE received\n");
}
