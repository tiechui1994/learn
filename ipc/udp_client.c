#include "unp.h"

int main(int argc, char** argv) {
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2) {
        err_quit("usage: udp_client <IP>");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    int n;
    char sendmsg[MAXLINE], recvmsg[MAXLINE];

    socklen_t servalen = sizeof(servaddr);
    while (Fgets(sendmsg, MAXLINE, stdin) != NULL) {
        Sendto(sockfd, sendmsg, strlen(sendmsg), 0, (struct sockaddr*) &servaddr, servalen);

        n = (int) Recvfrom(sockfd, recvmsg, MAXLINE, 0, NULL, NULL);
        if (n < 0) {
            break;
        }

        recvmsg[n] = 0;
        Fputs(recvmsg, stdout);
    }

    return 0;
}