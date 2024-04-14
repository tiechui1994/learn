#include "unp.h"

int main(int argc, char** argv) {
    int sockfd;
    if (argc != 3) {
        err_quit("usage: oob_tcpsend01 <host> <port>");
    }

    sockfd = Tcp_connect(argv[1], argv[2]);

    Write(sockfd, "123", 3);
    printf("wrote 3 bytes of normal data\n");
    sleep(1);

    Send(sockfd, "4", 1, MSG_OOB);
    printf("wrote 1 bytes of OOB data\n");
    sleep(1);

    Write(sockfd, "56", 2);
    printf("wrote 2 bytes of normal data\n");
    sleep(1);

    Send(sockfd, "7", 1, MSG_OOB);
    printf("wrote 1 bytes of OOB data\n");
    sleep(1);

    return 0;
}
