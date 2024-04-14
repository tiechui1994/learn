#include "unp.h"

int main(int argc, char** argv) {
    int listenfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET,
            servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);

    for (;;) {
        clilen = sizeof(cliaddr);
        connfd = Accept(listenfd, (struct sockaddr*) &cliaddr, &clilen);
        if (fork() == 0) {
            Close(listenfd);

            ssize_t n;
            char buf[MAXLINE];

            again:
            while ((n = Read(connfd, buf, MAXLINE)) > 0) {
                Writen(connfd, buf, (size_t) n);
            }

            if (n < 0 && errno == EINTR) {
                goto again;
            } else if (n < 0) {
                err_sys("read error");
            }

            exit(0);
        }
        Close(connfd);
    }
}


