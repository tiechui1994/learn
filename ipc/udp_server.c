#include "unp.h"

int main(int argc, char** argv) {
    int sockfd;
    struct sockaddr_in servaddr, clientaddr;

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr));

    int n;
    socklen_t len;
    char msg[MAXLINE];
    for (;;) {
        len = sizeof(clientaddr);
        n = (int) Recvfrom(sockfd, msg, MAXLINE, 0, (struct sockaddr*) &clientaddr, &len);
        if (n < 0) {
            break;
        }

        msg[n] = 0;
        printf("recv: %s", msg);

        Sendto(sockfd, msg, (size_t)n, 0, (struct sockaddr*) &clientaddr, len);
    }

    return 0;
}